include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

# These tests require C++
ifneq ($(CXX_FOR_TARGET),)
  SUBDIRS= cxx_throw libcpp
endif
