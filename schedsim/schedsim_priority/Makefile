#
#  $Id$
#

PREFIX=/home/jennifer/rtems-head-work/bsp-install/
OBJS = config.o printheir_executing.o schedsim.o wrap_thread_dispatch.o
CPPFLAGS = -I${PREFIX}/include/schedsim
LDFLAGS = -L${PREFIX}/lib
LDFLAGS += -Wl,--wrap=_Thread_Dispatch

all:    schedsim

schedsim: $(OBJS)
	g++ -o schedsim $(OBJS) $(LDFLAGS) -lschedsim -lrtems

clean:
	rm -f *.o schedsim
