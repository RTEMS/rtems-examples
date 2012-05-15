# Change this path to match your installation
PREFIX=/tmp/schedsim
OBJS = config.o printheir_executing.o schedsim.o wrap_thread_dispatch.o
CPPFLAGS = -I${PREFIX}/include/schedsim
LDFLAGS = -L${PREFIX}/lib
LDFLAGS += -Wl,--wrap=_Thread_Dispatch
SCHEDSIM=schedsim_priority

all:    stamp-test_prefix $(SCHEDSIM)

stamp-test_prefix:
	test -d ${PREFIX}
	test -f ${PREFIX}/include/schedsim/rtems_sched.h
	touch stamp-test_prefix

$(SCHEDSIM): $(OBJS)
	g++ -o $(SCHEDSIM) $(OBJS) $(LDFLAGS) -lschedsim -lrtems

clean:
	rm -f *.o $(SCHEDSIM) stamp-test_prefix
