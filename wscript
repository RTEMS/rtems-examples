# Copyright 2013 Chris Johns (chrisj@rtems.org)
#
# This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
#

# Waf build script for RTEMS examples
#
# To configure, build and run do:
#
# $ waf configure --rtems=/Users/chris/Development/rtems/build/4.11 \
#                 --rtems-tools=/Users/chris/Development/rtems/4.11 \
#                 --rtems-bsps=sparc/sis
# $ waf
# $ /Users/chris/Development/rtems/4.11/bin/sparc-rtems4.11-run ./build/sparc-rtems4.11-sis/hello
#
# You can use '--rtems-archs=sparc,i386' or '--rtems-bsps=sparc/sis,i386/pc586'
# to build for more than BSP at a time.
#

try:
    import rtems_waf.rtems as rtems
except:
    print 'error: no rtems_waf git submodule; see README.waf'
    import sys
    sys.exit(1)

def init(ctx):
    rtems.init(ctx)

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

def rebuild(ctx):
    import waflib.Options
    waflib.Options.commands.extend(['clean', 'build'])

def tags(ctx):
    ctx.exec_command('etags $(find . -name \*.[sSch])', shell = True)
