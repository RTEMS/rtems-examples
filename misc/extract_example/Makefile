#
#  Makefile to pull BSP specific information into a form the script can use
#

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc

include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

# Tool helpers
rtemsdir=${RTEMS_MAKEFILE_PATH}
CARGS += $(CPU_CFLAGS) $(LDFLAGS) $(AM_LDFLAGS)

all: RTEMS_SETTINGS

RTEMS_SETTINGS: 
	(echo "CC=$(CC)"; \
	 echo "AS=$(AS)"; \
	 echo "CPU_CFLAGS=$(CPU_CFLAGS)"; \
	 echo "CFLAGS=$(CFLAGS)" ; \
	 echo "LDFLAGS=$(LDFLAGS)" ; \
	 ) >$@
	@cat RTEMS_SETTINGS

clean:
	rm -f RTEMS_SETTINGS
