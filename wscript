# Copyright 2013 Chris Johns (chrisj@rtems.org)
#
# This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
#

from __future__ import print_function

# See README.waf for building instructions.

rtems_version = "5"

try:
    import rtems_waf.rtems as rtems
except:
    print('error: no rtems_waf git submodule; see README.waf')
    import sys
    sys.exit(1)

def init(ctx):
    rtems.init(ctx, version = rtems_version, long_commands = True)

def options(opt):
    rtems.options(opt)

def configure(conf):
    rtems.configure(conf)

def build(bld):
    rtems.build(bld)
    bld.env.CFLAGS += ['-O2','-g']
    bld.recurse('hello')
    bld.recurse('gdb')
    bld.recurse('filesystem/fat_ramdisk')
    bld.recurse('classic_api')
    bld.recurse('file_io')
    bld.recurse('ticker')
    bld.recurse('uboot')
    bld.recurse('led')
    bld.recurse('misc')
    bld.recurse('benchmarks')
    bld.recurse('micromonitor')
    bld.recurse('posix_api')
    bld.recurse('cxx')
    bld.recurse('c11')

def rebuild(ctx):
    import waflib.Options
    waflib.Options.commands.extend(['clean', 'build'])

def tags(ctx):
    ctx.exec_command('etags $(find . -name \*.[sSch])', shell = True)
