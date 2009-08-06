##########################################################################
#
# Basic, target/architecture independent makefile for building an
# application that runs on its own stack. 
# The application code in main.c provides a target-independent example
# of hooking up an application to a target running MicroMonitor.
# Also, there is a self-induced exception branch (see strace_demo.c)
# that can be invoked to force the application to take an exception.
# The purpose of this is to allow the user to then use the monitor's
# 'strace' command to see the function nesting that occurred prior to
# the exception.  This requires that the symbol table also be on-board.
# To walk through this example, establish the site-dependent information
# specified below, then execute "make" to build the image, and then
# "make dld sym" to install it (and a symbol table) on the target.
#
# Site dependent information:
# Adjust these values based on your system configuration.  
# ARCH:
# 	Set ARCH to one of the accepted CPU architectures (i.e. MIPS
#	PPC, ARM, BLACKFIN, COLDFIRE, MICROBLAZE).
# MONCOMPTR:
# 	Set MONCOMPTR to the output of 'echo $MONCOMPTR' on your target.
# APPRAMBASE:
# 	Set APPRAMBASE to the output of 'echo $APPRAMBASE' on your target.
# TARGET_IP:
#	Set TARGET_IP to the IP address of your target.
#
ARCH		= ARM
MONCOMPTR	= 0x10000020
APPRAMBASE	= 0x20100000
TARGET_IP	= 192.168.1.242

##########################################################################
#
# There should be no need to change anything below this point if
# building for the csb350, csb472, csb337 or csb360...
# 
APPNAME		= diagnostics
NM			= $(TOOL_PREFIX)-nm
AR			= $(TOOL_PREFIX)-ar
LD			= $(TOOL_PREFIX)-ld
ASM			= $(TOOL_PREFIX)-as
CC			= $(TOOL_PREFIX)-gcc
STRIP		= $(TOOL_PREFIX)-strip
OBJCOPY		= $(TOOL_PREFIX)-objcopy
OBJDUMP		= $(TOOL_PREFIX)-objdump
LIBGCC		= `$(CC) --print-libgcc-file-name`
LIBDIR		= $(LIBGCC:/libgcc.a=)
LIBPATH		= 

ifeq ($(ARCH),MIPS)
TOOL_PREFIX	:= mips-elf
CFLAGS		:= -fno-builtin -G 0 -march=r4600 -mips3 -mno-abicalls \
			  -fno-pic -c -g -O2 -Wall -EB -I . 
CRT0		:= crt0_mips.o
CPU			:= -D CPU_IS_MIPS=1
endif

ifeq ($(ARCH),PPC)
TOOL_PREFIX	:= ppc-elf
CFLAGS		:= -fno-builtin -mno-sdata -msoft-float \
			   -c -Wall -O -g -I. 
CRT0		:= crt0_ppc.o
CPU			:= -D CPU_IS_PPC=1
LIBGCC		= `$(CC) --print-file-name=nof/libgcc.a`
endif

ifeq ($(ARCH),ARM)
#TOOL_PREFIX	:= arm-elf
TOOL_PREFIX	:= arm-rtems4.10
CFLAGS		:= -fno-builtin -mcpu=arm920t -fno-omit-frame-pointer \
			   -c -Wall -O -g -I.
CRT0		:= crt0_arm.o
CPU			:= -D CPU_IS_ARM=1
endif

ifeq ($(ARCH),BLACKFIN)
TOOL_PREFIX	:= bfin-elf
CFLAGS		:= -fno-builtin -mcsync-anomaly -c -Wall -O -g -I.
CRT0		:= crt0_bfin.o
CPU			:= -D CPU_IS_BFIN=1
endif

ifeq ($(ARCH),MICROBLAZE)
TOOL_PREFIX	:= C:/EDK/gnu/microblaze/nt/bin/mb
LIBPATH		:= -L C:/xilinx/els_stuff/projects/avnet_spartan3_devkit/microblaze_0/lib
CFLAGS		:= -fno-builtin -mno-xl-soft-mul -c -Wall -O -g -I.
CRT0		:= crt0_mb.o
CPU			:= -D CPU_IS_MICROBLAZE=1
endif

ifeq ($(ARCH),COLDFIRE)
TOOL_PREFIX	:= m68k-elf
CFLAGS		:= -Wall -fno-builtin -msoft-float -m5200 -g -c -I.
CRT0		:= crt0_cf.o
CPU			:= -D CPU_IS_68K=1
#LIBGCC		= `$(CC) -m5200 --print-libgcc-file-name`
LIBGCC		= /usr/lib/gcc-lib/m68k-elf/3.2/m5200/libgcc.a -L /usr/m68k-elf/lib/m5200
endif

START=start

CRT0=cstart.o
START=Cstart
OBJS=$(CRT0) main.o monlib.o

all: $(APPNAME)
#####
#
# $(APPNAME):
# Top level target builds the application.
#
$(APPNAME): varcheck $(OBJS) makefile
	echo tools: $(TOOL_PREFIX)
	$(LD) -e $(START) -o $(APPNAME) -Ttext $(APPRAMBASE) $(OBJS) $(LIBPATH) -lc $(LIBGCC) 
	$(NM) --numeric-sort $(APPNAME) >$(APPNAME).sym
	$(OBJDUMP) --source --disassemble $(APPNAME) > $(APPNAME).dis
	$(STRIP) $(APPNAME) 

#####
#
# Variable checks:
# Verify that the necessary variables have been set on the make
# command line.
#
varcheck:
ifndef ARCH
	@echo Must specify ARCH=XXX on command line.
	@exit 1
endif	
ifndef TOOL_PREFIX
	@echo Invalid ARCH specification. Use PPC, ARM, MIPS, BLACKFIN or COLDFIRE.
	@exit 1
endif	
ifeq ($(TOOL_PREFIX),-)
	@echo Invalid ARCH specification. Use PPC, ARM, MIPS, BLACKFIN or COLDFIRE.
	@exit 1
endif	
ifndef MONCOMPTR
	@echo Must specify MONCOMPTR=XXX on command line.
	@exit 1
endif	
ifndef APPRAMBASE
	@echo Must specify APPRAMBASE=XXX on command line.
	@exit 1
endif	

targetipcheck:
ifndef TARGET_IP
	@echo Must specify TARGET_IP=IPADDRESS on command line.
	@exit 1
endif	


#####
#
# Objects:
#
crt0_68k.o: crt0_68k.S
	$(CC) $(CFLAGS) -o $@ crt0_68k.S

crt0_arm.o: crt0_arm.S
	$(CC) $(CFLAGS) -o $@ crt0_arm.S

crt0_bfin.o: crt0_bfin.S
	$(CC) $(CFLAGS) -o $@ crt0_bfin.S

crt0_mips.o: crt0_mips.S
	$(CC) $(CFLAGS) -o $@ crt0_mips.S

crt0_mb.o: crt0_mb.S
	$(CC) $(CFLAGS) -o $@ crt0_mb.S

crt0_ppc.o: crt0_ppc.S
	$(CC) $(CFLAGS) -o $@ crt0_ppc.S

crt0_sh2.o: crt0_sh2.S
	$(CC) $(CFLAGS) -o $@ crt0_sh2.S

main.o: main.c 
	$(CC) $(CFLAGS) -D MONCOMPTR=$(MONCOMPTR) -o $@ main.c

cstart.o: cstart.c 
	$(CC) $(CFLAGS) -D MONCOMPTR=$(MONCOMPTR) -o $@ cstart.c

monlib.o: monlib.c
	$(CC) $(CFLAGS) -o $@ monlib.c

strace.o: strace.c 
	$(CC) $(CFLAGS) $(CPU) -o $@ strace.c

#####
#
# clean:
# Remove all files created by this make.
#
clean:
	rm -f *.o $(APPNAME) $(APPNAME).ezip $(APPNAME).sym $(APPNAME).dis symtbl

#####
#
# sym:
# Create and download the symbol table file that can be used by uMon
# with this application...
#
sym: targetipcheck
	@if ! test -f $(APPNAME).sym; then echo Must build $(APPNAME) first; exit 1; fi
	monsym -p0x $(APPNAME).sym >symtbl
	ttftp $(TARGET_IP) put symtbl


	
#####
#
# dld:
# Use the ttftp tool (supplied with MicroMonitor) to download the
# application to the target.
#
dld: targetipcheck
	@if ! test -f $(APPNAME); then echo Must build $(APPNAME) first; exit 1; fi
	ttftp $(TARGET_IP) put $(APPNAME) $(APPNAME),E

#####
#
# zdld:
# Compress the elf file using the 'elf' tool (supplied with MicroMonitor)
# The output of this is "$(APPNAME).ezip", then download that compressed file.
#
zdld: targetipcheck 
	@if ! test -f $(APPNAME); then echo Must build $(APPNAME) first; exit 1; fi
	elf -z6 $(APPNAME)
	ttftp $(TARGET_IP) put $(APPNAME).ezip $(APPNAME),Ec
