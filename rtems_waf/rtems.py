#
# Copyright 2012, 2013 Chris Johns (chrisj@rtems.org)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.

# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# RTEMS support for applications.
#

import copy
import os
import os.path
import pkgconfig
import re
import subprocess

default_version = '4.11'
default_label = 'rtems-' + default_version
default_path = '/opt/' + default_label
default_postfix = 'rtems' + default_version

rtems_filters = None

def options(opt):
    opt.add_option('--rtems',
                   default = default_path,
                   dest = 'rtems_path',
                   help = 'Path to an installed RTEMS.')
    opt.add_option('--rtems-tools',
                   default = None,
                   dest = 'rtems_tools',
                   help = 'Path to RTEMS tools.')
    opt.add_option('--rtems-version',
                   default = default_version,
                   dest = 'rtems_version',
                   help = 'RTEMS version (default ' + default_version + ').')
    opt.add_option('--rtems-archs',
                   default = 'all',
                   dest = 'rtems_archs',
                   help = 'List of RTEMS architectures to build.')
    opt.add_option('--rtems-bsps',
                   default = 'all',
                   dest = 'rtems_bsps',
                   help = 'List of BSPs to build.')
    opt.add_option('--show-commands',
                   action = 'store_true',
                   default = False,
                   dest = 'show_commands',
                   help = 'Print the commands as strings.')

def init(ctx, filters = None):
    global rtems_filters

    try:
        import waflib.Options
        import waflib.ConfigSet

        #
        # Load the configuation set from the lock file.
        #
        env = waflib.ConfigSet.ConfigSet()
        env.load(waflib.Options.lockfile)

        #
        # Set the RTEMS filter to the context.
        #
        rtems_filters = filters

        #
        # Check the tools, architectures and bsps.
        #
        rtems_path, rtems_tools, archs, arch_bsps = check_options(ctx,
                                                                  env.options['rtems_tools'],
                                                                  env.options['rtems_path'],
                                                                  env.options['rtems_version'],
                                                                  env.options['rtems_archs'],
                                                                  env.options['rtems_bsps'])

        #
        # Update the contextes for all the bsps.
        #
        from waflib.Build import BuildContext, CleanContext, \
            InstallContext, UninstallContext
        for x in arch_bsps:
            for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
                name = y.__name__.replace('Context','').lower()
                class context(y):
                    cmd = name + '-' + x
                    variant = x

        #
        # Add the various commands.
        #
        for cmd in ['build', 'clean', 'install']:
            if cmd in waflib.Options.commands:
                waflib.Options.commands.remove(cmd)
                for x in arch_bsps:
                    waflib.Options.commands.insert(0, cmd + '-' + x)
    except:
        pass

def configure(conf):
    #
    # Handle the show commands option.
    #
    if conf.options.show_commands:
        show_commands = 'yes'
    else:
        show_commands = 'no'

    rtems_path, rtems_tools, archs, arch_bsps = check_options(conf,
                                                              conf.options.rtems_tools,
                                                              conf.options.rtems_path,
                                                              conf.options.rtems_version,
                                                              conf.options.rtems_archs,
                                                              conf.options.rtems_bsps)

    _log_header(conf)

    conf.msg('Architectures', ', '.join(archs), 'YELLOW')

    tools = {}
    env = conf.env.derive()

    for ab in arch_bsps:
        conf.setenv(ab, env)

        conf.msg('Board Support Package', ab, 'YELLOW')

        arch = _arch_from_arch_bsp(ab)

        conf.env.RTEMS_PATH = conf.options.rtems_path
        conf.env.RTEMS_VERSION = conf.options.rtems_version
        conf.env.RTEMS_ARCH_BSP = ab
        conf.env.RTEMS_ARCH = arch.split('-')[0]
        conf.env.RTEMS_ARCH_RTEMS = arch
        conf.env.RTEMS_BSP = _bsp_from_arch_bsp(ab)

        tools = _find_tools(conf, arch, [rtems_path] + rtems_tools, tools)
        for t in tools[arch]:
            conf.env[t] = tools[arch][t]

        conf.load('gcc')
        conf.load('g++')
        conf.load('gas')

        flags = _load_flags(conf, ab, conf.options.rtems_path)

        cflags = _filter_flags('cflags', flags['CFLAGS'],
                               arch, conf.options.rtems_path)
        ldflags = _filter_flags('ldflags', flags['LDFLAGS'],
                               arch, conf.options.rtems_path)

        conf.env.CFLAGS    = cflags['cflags']
        conf.env.CXXFLAGS  = conf.env.CFLAGS
        conf.env.ASFLAGS   = cflags['cflags']
        conf.env.WFLAGS    = cflags['warnings']
        conf.env.RFLAGS    = cflags['specs']
        conf.env.MFLAGS    = cflags['machines']
        conf.env.IFLAGS    = cflags['includes']
        conf.env.LINKFLAGS = cflags['cflags'] + ldflags['ldflags']
        conf.env.LIB       = flags['LIB']

        #
        # Add tweaks.
        #
        tweaks(conf, ab)

        conf.env.SHOW_COMMANDS = show_commands

        conf.setenv('', env)

    conf.env.RTEMS_TOOLS = rtems_tools
    conf.env.ARCHS = archs
    conf.env.ARCH_BSPS = arch_bsps

    conf.env.SHOW_COMMANDS = show_commands

def build(bld):
    if bld.env.SHOW_COMMANDS == 'yes':
        output_command_line()

def tweaks(conf, arch_bsp):
    #
    # Hack to work around NIOS2 naming.
    #
    if conf.env.RTEMS_ARCH in ['nios2']:
        conf.env.OBJCOPY_FLAGS = ['-O', 'elf32-littlenios2']
    elif conf.env.RTEMS_ARCH in ['arm']:
        conf.env.OBJCOPY_FLAGS = ['-I', 'binary', '-O', 'elf32-littlearm']
    else:
        conf.env.OBJCOPY_FLAGS = ['-O', 'elf32-' + conf.env.RTEMS_ARCH]

    #
    # Check for a i386 PC bsp.
    #
    if re.match('i386-.*-pc[3456]86', arch_bsp) is not None:
        conf.env.LINKFLAGS += ['-Wl,-Ttext,0x00100000']

    if '-ffunction-sections' in conf.env.CFLAGS:
      conf.env.LINKFLAGS += ['-Wl,--gc-sections']

def check_options(ctx, rtems_tools, rtems_path, rtems_version, rtems_archs, rtems_bsps):
    #
    # Check the paths are valid.
    #
    if not os.path.exists(rtems_path):
        ctx.fatal('RTEMS path not found.')
    if os.path.exists(os.path.join(rtems_path, 'lib', 'pkgconfig')):
        rtems_config = None
    elif os.path.exists(os.path.join(rtems_path, 'rtems-config')):
        rtems_config = os.path.join(rtems_path, 'rtems-config')
    else:
        ctx.fatal('RTEMS path is not valid. No lib/pkgconfig or rtems-config found.')
    if os.path.exists(os.path.join(rtems_path, 'bin')):
        rtems_bin = os.path.join(rtems_path, 'bin')
    else:
        ctx.fatal('RTEMS path is not valid. No bin directory found.')

    #
    # We can more than one path to tools. This happens when testing different
    # versions.
    #
    if rtems_tools is not None:
        rt = rtems_tools.split(',')
        tools = []
        for path in rt:
            if not os.path.exists(path):
                ctx.fatal('RTEMS tools path not found: ' + path)
            if not os.path.exists(os.path.join(path, 'bin')):
                ctx.fatal('RTEMS tools path does not contain a \'bin\' directory: ' + path)
            tools += [os.path.join(path, 'bin')]
    else:
        tools = None

    #
    # Filter the tools.
    #
    tools = filter(ctx, 'tools', tools)

    #
    # Match the archs requested against the ones found. If the user
    # wants all (default) set all used.
    #
    if rtems_archs == 'all':
        archs = _find_installed_archs(rtems_config, rtems_path, rtems_version)
    else:
        archs = _check_archs(rtems_config, rtems_archs, rtems_path, rtems_version)

    #
    # Filter the architectures.
    #
    archs = filter(ctx, 'archs', archs)

    #
    # We some.
    #
    if len(archs) == 0:
        ctx.fatal('Could not find any architectures')

    #
    # Get the list of valid BSPs. This process filters the architectures
    # to those referenced by the BSPs.
    #
    if rtems_bsps == 'all':
        arch_bsps = _find_installed_arch_bsps(rtems_config, rtems_path, archs, rtems_version)
    else:
        arch_bsps = _check_arch_bsps(rtems_bsps, rtems_config, rtems_path, archs, rtems_version)

    if len(arch_bsps) == 0:
        ctx.fatal('No valid arch/bsps found')

    #
    # Filter the bsps.
    #
    arch_bsps = filter(ctx, 'bsps', arch_bsps)

    return rtems_bin, tools, archs, arch_bsps

def arch(arch_bsp):
    """ Given an arch/bsp return the architecture."""
    return _arch_from_arch_bsp(arch_bsp).split('-')[0]

def bsp(arch_bsp):
    """ Given an arch/bsp return the BSP."""
    return _bsp_from_arch_bsp(arch_bsp)

def arch_bsps(ctx):
    """ Return the list of arch/bsps we are building."""
    return ctx.env.ARCH_BSPS

def arch_bsp_env(ctx, arch_bsp):
    return ctx.env_of_name(arch_bsp).derive()

def filter(ctx, filter, items):
    if rtems_filters is None:
        return items
    if type(rtems_filters) is not dict:
        ctx.fatal("Invalid RTEMS filter type, ie { 'tools': { 'in': [], 'out': [] }, 'arch': {}, 'bsps': {} }")
    if filter not in rtems_filters:
        return items
    items_in = []
    items_out = []
    if 'in' in rtems_filters[filter]:
        items_in = copy.copy(rtems_filters[filter]['in'])
    if 'out' in rtems_filters[filter]:
        items_out = copy.copy(rtems_filters[filter]['out'])
    filtered_items = []
    for i in items:
        item = i
        ab = '%s/%s' % (arch(item), bsp(item))
        for inre in items_in:
            if re.compile(inre).match(ab):
                items_in.remove(inre)
                filtered_items += [item]
                item = None
                break
        if item is not None:
            for outre in items_out:
                if re.compile(outre).match(ab):
                    item = None
                    break
        if item is not None:
            filtered_items += [item]
    if len(items_in) != 0:
        ctx.fatal('Following %s not found: %s' % (filter, ', '.join(items_in)))
    return sorted(filtered_items)

def arch_rtems_version(arch):
    """ Return the RTEMS architecture path, ie sparc-rtems4.11."""
    return '%s-%s' % (arch, default_postfix)

def arch_bsp_path(arch_bsp):
    """ Return the BSP path."""
    return '%s/%s' % (arch_rtems_version(arch(arch_bsp)), bsp(arch_bsp))

def arch_bsp_include_path(arch_bsp):
    """ Return the BSP include path."""
    return '%s/lib/include' % (arch_bsp_path(arch_bsp))

def arch_bsp_lib_path(arch_bsp):
    """ Return the BSP library path. """
    return '%s/lib' % (arch_bsp_path(arch_bsp))

def library_path(library, cc, cflags):
    cmd = cc + cflags + ['-print-file-name=%s' % library]
    a = subprocess.check_output(cmd)
    lib = os.path.abspath(a.strip())
    if os.path.exists(lib):
        return os.path.dirname(lib)
    return None

def clone_tasks(bld):
    if bld.cmd == 'build':
        for obj in bld.all_task_gen[:]:
            for x in arch_bsp:
                cloned_obj = obj.clone(x)
                kind = Options.options.build_kind
                if kind.find(x) < 0:
                    cloned_obj.posted = True
            obj.posted = True

#
# From the demos. Use this to get the command to cut+paste to play.
#
def output_command_line():
    # first, display strings, people like them
    from waflib import Utils, Logs
    from waflib.Context import Context
    def exec_command(self, cmd, **kw):
        subprocess = Utils.subprocess
        kw['shell'] = isinstance(cmd, str)
        if isinstance(cmd, str):
            Logs.info('%s' % cmd)
        else:
            Logs.info('%s' % ' '.join(cmd)) # here is the change
        Logs.debug('runner_env: kw=%s' % kw)
        try:
            if self.logger:
                self.logger.info(cmd)
                kw['stdout'] = kw['stderr'] = subprocess.PIPE
                p = subprocess.Popen(cmd, **kw)
                (out, err) = p.communicate()
                if out:
                    self.logger.debug('out: %s' % out.decode(sys.stdout.encoding or 'iso8859-1'))
                if err:
                    self.logger.error('err: %s' % err.decode(sys.stdout.encoding or 'iso8859-1'))
                return p.returncode
            else:
                p = subprocess.Popen(cmd, **kw)
                return p.wait()
        except OSError:
            return -1
    Context.exec_command = exec_command

    # Change the outputs for tasks too
    from waflib.Task import Task
    def display(self):
        return '' # no output on empty strings

    Task.__str__ = display

def _find_tools(conf, arch, paths, tools):
    if arch not in tools:
        arch_tools = {}
        arch_tools['CC']          = conf.find_program([arch + '-gcc'], path_list = paths)
        arch_tools['CXX']         = conf.find_program([arch + '-g++'], path_list = paths)
        arch_tools['AS']          = conf.find_program([arch + '-gcc'], path_list = paths)
        arch_tools['LD']          = conf.find_program([arch + '-ld'],  path_list = paths)
        arch_tools['AR']          = conf.find_program([arch + '-ar'],  path_list = paths)
        arch_tools['LINK_CC']     = arch_tools['CC']
        arch_tools['LINK_CXX']    = arch_tools['CXX']
        arch_tools['AR']          = conf.find_program([arch + '-ar'], path_list = paths)
        arch_tools['LD']          = conf.find_program([arch + '-ld'], path_list = paths)
        arch_tools['NM']          = conf.find_program([arch + '-nm'], path_list = paths)
        arch_tools['OBJDUMP']     = conf.find_program([arch + '-objdump'], path_list = paths)
        arch_tools['OBJCOPY']     = conf.find_program([arch + '-objcopy'], path_list = paths)
        arch_tools['READELF']     = conf.find_program([arch + '-readelf'], path_list = paths)
        arch_tools['STRIP']       = conf.find_program([arch + '-strip'], path_list = paths)
        arch_tools['RTEMS_LD']    = conf.find_program(['rtems-ld'], path_list = paths, mandatory = False)
        arch_tools['RTEMS_BIN2C'] = conf.find_program(['rtems-bin2c'], path_list = paths, mandatory = False)
        arch_tools['TAR']         = conf.find_program(['tar'], mandatory = False)
        tools[arch] = arch_tools
    return tools

def _find_installed_archs(config, path, version):
    archs = []
    if config is None:
        for d in os.listdir(path):
            if d.endswith('-rtems' + version):
                archs += [d]
    else:
        a = subprocess.check_output([config, '--list-format', '"%(arch)s"'])
        a = a[:-1].replace('"', '')
        archs = set(a.split())
        archs = ['%s-rtems4.11' % (x) for x in archs]
    archs.sort()
    return archs

def _check_archs(config, req, path, version):
    installed = _find_installed_archs(config, path, version)
    archs = []
    for a in req.split(','):
        arch = a + '-rtems' + version
        if arch in installed:
            archs += [arch]
    archs.sort()
    return archs

def _find_installed_arch_bsps(config, path, archs, version):
    arch_bsps = []
    if config is None:
        for f in os.listdir(_pkgconfig_path(path)):
            if f.endswith('.pc'):
                if _arch_from_arch_bsp(f[:-3]) in archs:
                    arch_bsps += [f[:-3]]
    else:
        ab = subprocess.check_output([config, '--list-format'])
        ab = ab[:-1].replace('"', '')
        ab = ab.replace('/', '-rtems%s-' % (version))
        arch_bsps = [x for x in set(ab.split())]
    arch_bsps.sort()
    return arch_bsps

def _check_arch_bsps(req, config, path, archs, version):
    archs_bsps = []
    for ab in req.split(','):
        abl = ab.split('/')
        if len(abl) != 2:
            return []
        found = False
        for arch in archs:
            a = '%s-rtems%s' % (abl[0], version)
            if a == arch:
                found = True
                break
        if not found:
            return []
        archs_bsps += ['%s-%s' % (a, abl[1])]
    if len(archs_bsps) == 0:
        return []
    installed = _find_installed_arch_bsps(config, path, archs, version)
    bsps = []
    for b in archs_bsps:
        if b in installed:
            bsps += [b]
    bsps.sort()
    return bsps

def _arch_from_arch_bsp(arch_bsp):
    return '-'.join(arch_bsp.split('-')[:2])

def _bsp_from_arch_bsp(arch_bsp):
    return '-'.join(arch_bsp.split('-')[2:])

def _pkgconfig_path(path):
    return os.path.join(path, 'lib', 'pkgconfig')

def _load_flags(conf, arch_bsp, path):
    if not os.path.exists(path):
        ctx.fatal('RTEMS path not found.')
    if os.path.exists(_pkgconfig_path(path)):
        pc = os.path.join(_pkgconfig_path(path), arch_bsp + '.pc')
        conf.to_log('Opening and load pkgconfig: ' + pc)
        pkg = pkgconfig.package(pc)
        config = None
    elif os.path.exists(os.path.join(path, 'rtems-config')):
        config = os.path.join(path, 'rtems-config')
        pkg = None
    flags = {}
    _log_header(conf)
    flags['CFLAGS'] = _load_flags_set('CFLAGS', arch_bsp, conf, config, pkg)
    flags['LDFLAGS'] = _load_flags_set('LDFLAGS', arch_bsp, conf, config, pkg)
    flags['LIB'] = _load_flags_set('LIB', arch_bsp, conf, config, pkg)
    return flags

def _load_flags_set(flags, arch_bsp, conf, config, pkg):
    conf.to_log('%s ->' % flags)
    if pkg is not None:
        flagstr = ''
        try:
            flagstr = pkg.get(flags)
        except pkgconfig.error as e:
            conf.to_log('pkconfig warning: ' + e.msg)
        conf.to_log('  ' + flagstr)
    else:
        flags_map = { 'CFLAGS': '--cflags',
                      'LDFLAGS': '--ldflags',
                      'LIB': '--libs' }
        ab = arch_bsp.split('-')
        #conf.check_cfg(path = config,
        #               package = '',
        #               uselib_store = 'rtems',
        #               args = '--bsp %s/%s %s' % (ab[0], ab[2], flags_map[flags]))
        #print conf.env
        #print '%r' % conf
        #flagstr = '-l -c'
        flagstr = subprocess.check_output([config, '--bsp', '%s/%s' % (ab[0], ab[2]), flags_map[flags]])
        #print flags, ">>>>", flagstr
        if flags == 'CFLAGS':
            flagstr += ' -DWAF_BUILD=1'
        if flags == 'LIB':
            flagstr = 'rtemscpu rtemsbsp c rtemscpu rtemsbsp'
    return flagstr.split()

def _filter_flags(label, flags, arch, rtems_path):

    flag_groups = \
        [ { 'key': 'warnings', 'path': False, 'flags': { '-W': 1 }, 'cflags': False, 'lflags': False },
          { 'key': 'includes', 'path': True,  'flags': { '-I': 1, '-isystem': 2, '-sysroot': 2 } },
          { 'key': 'machines', 'path': True,  'flags': { '-O': 1, '-m': 1, '-f': 1 } },
          { 'key': 'specs',    'path': True,  'flags': { '-q': 1, '-B': 2, '--specs': 2 } } ]

    flags = _strip_cflags(flags)

    _flags = { label: [] }
    for fg in flag_groups:
        _flags[fg['key']] = []

    iflags = iter(flags)
    for opt in iflags:
        in_label = True
        opts = []
        for fg in flag_groups:
            key = fg['key']
            for flag in fg['flags']:
                if opt.startswith(flag):
                    opt_count = fg['flags'][flag]
                    if opt_count > 1:
                        if opt != flag:
                            opt_count -= 1
                            if fg['path'] and arch in opt:
                                opt = '%s%s/%s' % (flag, rtems_path,
                                                   opt[opt.find(arch):])
                    opts += [opt]
                    for c in range(1, opt_count):
                        opt = next(iflags)
                        if fg['path'] and arch in opt:
                            opt = '%s%s/%s' % (f, rtems_path,
                                               opt[opt.find(arch):])
                        opts += [opt]
                    _flags[key] += opts
                    if label in fg and not fg[label]:
                        in_label = False
                    break
            if in_label:
                _flags[label] += opts
    return _flags

def _strip_cflags(cflags):
    _cflags = []
    for o in cflags:
        if o.startswith('-O'):
            pass
        elif o.startswith('-g'):
            pass
        else:
            _cflags += [o]
    return _cflags

def _log_header(conf):
    conf.to_log('-----------------------------------------')

from waflib import TaskGen
from waflib.Tools.ccroot import link_task, USELIB_VARS
USELIB_VARS['rap'] = set(['RTEMS_LINKFLAGS'])
@TaskGen.extension('.c')
class rap(link_task):
        "Link object files into a RTEMS application"
        run_str = '${RTEMS_LD} ${RTEMS_LINKFLAGS} --cc ${CC} ${SRC} -o ${TGT[0].abspath()} ${STLIB_MARKER} ${STLIBPATH_ST:STLIBPATH} ${STLIB_ST:STLIB} ${LIBPATH_ST:LIBPATH} ${LIB_ST:LIB}'
        ext_out = ['.rap']
        vars    = ['RTEMS_LINKFLAGS', 'LINKDEPS']
        inst_to = '${BINDIR}'
