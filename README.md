RTEMS Examples
==============

This is a set of example RTEMS programs.  Each subdirectory
reflects a set of related examples.


| Directory    | Description                                             |
| ---          | ---                                                     |
| benchmarks   | Collection of various public benchmark program          |
| classic_api  | Classic API examples (no LED)                           |
| cxx          | C++ examples                                            |
| file_io      | File I/O                                                |
| filesystem   | File systems, check inside for more details             |
| gdb          | Programs to exercise GDB features                       |
| hello        | Hello World Variations                                  |
| led          | LED Blinker Varations                                   |
| lvgl_hello   | LittleVGL graphics app example using libbsd framebuffer |
| micromonitor | MicroMonitor interaction examples                       |
| misc         | Miscellaneous                                           |
| posix_api    | POSIX API examples (no led)                             |
| schedsim     | RTEMS Scheduler Simulator examples                      |
| ticker       | Ticker Variations                                       |
| uboot        | U-Boot interaction examples                             |


RTEMS Waf
=========

You can find the Waf project here:

  * https://waf.io

Simple instructions on How to set up Waf is here:

  * http://www.rtems.org/ftp/pub/rtems/people/chrisj/rtl/rtems-linker/waf.html

Steps
-----

  1. Build or install the tools. In this example the path is
     `$HOME/Development/rtems/5`.

  2. Build and install the RTEMS Board Support Packages you want to use. In this
     example the path is `$HOME/Development/rtems/build/5`.

  3. Unpack this package somewhere, anywhere on your disk and change into the top
     level directory.

  4. Populate the git submodule:

     ```shell
     $ git submodule init
     $ git submodule update
     ```

  5. Configure with your specific settings. In this case the path to the tools
     and RTEMS and provided on the command line and so do not need to be in your
     path or environment [^1] and we limit the build to a single BSP. The single
     BSP should be the `sparc/erc32` BSP since this is the easiest BSP to use
     on a simulator. After you have successfully build the executables for this
     BSP and run them on the simulator, you can repeat this exercise with the
     BSP you are most interested in.

     ```shell
     $ waf configure --rtems=$HOME/Development/rtems/build/5 \
                     --rtems-tools=$HOME/Development/rtems/5 \
                     --rtems-bsps=sparc/erc32
     ```


     You can use these to build more than one BSP at a time:

     ```shell
     $ waf configure --rtems-archs=sparc,i386
     $ waf configure --rtems-bsps=sparc/erc32,i386/pc586
     ```

  6. Build:

     ```shell
     $ waf
     ```

  7. Run the executable using the simulator

     ```shell
      $ $HOME/Development/rtems/5/bin/sparc-rtems5-run \
        ./build/sparc-rtems5-erc32/hello
     ```

[^1]: It is good practice to keep your environment as empty as possible. Setting
      paths to tools or specific values to configure or control builds is
      dangerous because settings can leak between different builds and change
      what you expect a build to do. The waf tool used here lets you specify on
      the command line the tools and RTEMS paths and this is embedded in waf's
      configuration information. If you have a few source trees working at any
      one time with different tool sets or configurations you can easly move
      between them safe in the knowledge that one build will not infect another.


Makefile
========

The Makefile system used in these examples is dependent on the
environment variable RTEMS_MAKEFILE_PATH being set. This can be
set on each command line that invokes "make" or it can be exported
into your environment.

RTEMS_MAKEFILE_PATH points to the installed BSP image you are compiling
for. It is composed as follows:

  ```shell
  @prefix@/@target@/BSP
  ```

where:

  * Prefix is the BSP install point or prefix
  * Target is the tool target (e.g. sparc-rtems5)
  * BSP is the BSP you are building for (e.g. erc32)

A coupte of examples:

  ```shell
  export RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32
  make clean
  make
  ```

OR:

  ```shell
  RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32 make clean
  RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32 make
  ```

You can switch a build from one BSP to another by changing the value
of RTEMS_MAKEFILE_PATH but be careful to "make clean" on the old BSP
before building the new one.

Deprecration Warning: This style of Makefile has been included in RTEMS
since the earliest days. However, it is being replaced by the use of waf
and something akin to pkgconfig to obtain compilation settings. The intent
is to make it easier for users to configure their preferred build system
for an RTEMS application.
