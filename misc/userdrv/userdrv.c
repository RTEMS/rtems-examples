/**
 * @file
 * @brief Example User Driver
 *
 * @ingroup user_driver Device Driver
 * @brief Example User Device Driver Implementation
 */

/*
 * Copyright (C) 2022 OAR Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <rtems.h>
#include "userdrv.h"
#include <rtems/libio.h>
#include <rtems/bspIo.h>

#include <pthread.h>
#include <string.h>

/** 
 * This is the major number for this device driver. It is stored during
 * the initialize entry. It is not used in this driver but may be of use
 * in other cases.
 */
static uint32_t    USERDRV_major;

/**
 * This variable indicates whether the driver has been initialized.
 */
static char initialized = 0;

rtems_device_driver userdrv_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor RTEMS_UNUSED,
  void                      *pargp RTEMS_UNUSED
)
{
  rtems_device_driver status;

  printk("  userdrv_initialize\n");

  /**
   * @internal
   *
   * If this is the first call to the initialize entry point, then register
   * the device name(s) for this driver.
   *
   * If any device specific initialization needs to be done, this is the time.
   * Initialization of specific instances sometimes can be deferred until the
   * open entry point.
   */
 
  if ( !initialized ) {
    initialized = 1;

    status = rtems_io_register_name(
      "/dev/userdrv",
      major,
      (rtems_device_minor_number) 0
    );
    if (status != RTEMS_SUCCESSFUL)
      rtems_fatal_error_occurred(status);

    USERDRV_major = major;
  }

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver userdrv_open(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor RTEMS_UNUSED,
  void                      *pargp RTEMS_UNUSED
)
{
  printk("  userdrv_open\n");

  /**
   * @internal
   *
   * In a more realistic driver, the open entry would validate the minor
   * number passed in as representing a valid device instance. It may also
   * keep track of multiple opens and support the device staying open until
   * the last close.
   *
   * There may be some hardware initialization that occurs related to the
   * device instance at this point. For example, a termios driver would
   * register its callbacks with the termios framework and initialize a UART
   * with the default settings.
   */
 
  return RTEMS_SUCCESSFUL;
}

rtems_device_driver userdrv_close(
  rtems_device_major_number major RTEMS_UNUSED,
  rtems_device_minor_number minor RTEMS_UNUSED,
  void *pargp RTEMS_UNUSED
)
{
  printk("  userdrv_close\n");

  /**
   * @internal
   *
   * In a more realistic driver, the close entry would validate the minor
   * number passed in as representing a valid device instance. It would also
   * verify that the device is open before closing it. If tracking nested
   * open and close operations on the same device instance, that would also
   * be managed here.
   *
   * There may be some shutdown for the device instance. For example, it
   * may be necessary to disable intterrupts from this device instance.
   */
  return RTEMS_SUCCESSFUL;
}

rtems_device_driver userdrv_read(
  rtems_device_major_number major RTEMS_UNUSED,
  rtems_device_minor_number minor RTEMS_UNUSED,
  void *pargp RTEMS_UNUSED
)
{
  rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) pargp;

  printk("  userdrv_read\n");

  /**
   * @internal
   *
   * In a more realistic driver, the read entry would validate the minor
   * number passed in as representing a valid device instance. There is
   * likely more error checking on the arguments passed in. But it should
   * be safe to trust the file system framework to pass the @a argp buffer
   * as defined.
   */

  if ( rw_args == NULL ) {
    printk("  nothing to read\n");
  }

  if ( rw_args->count == 0 ) {
    printk("  buffer of zero\n");
  }

  /**
   * @internal
   *
   * A functional driver would replace this block with code that accesses
   * the device instance and returns real data if any is available.
   */
  const char *outstr = "this is output";
  size_t outlen = 15;

  rw_args->bytes_moved = outlen;
  memcpy(rw_args->buffer, outstr, outlen);

  printk("  %d buffer size\n", rw_args->count);
  printk("  buffer: %s\n", rw_args->buffer);

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver userdrv_write(
  rtems_device_major_number major RTEMS_UNUSED,
  rtems_device_minor_number minor RTEMS_UNUSED,
  void *pargp
)
{
  rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) pargp;

  printk("  userdrv_write\n");

  /**
   * @internal
   *
   * In a more realistic driver, the write entry would validate the minor
   * number passed in as representing a valid device instance. There is
   * likely more error checking on the arguments passed in. But it should
   * be safe to trust the file system framework to pass the @a argp buffer
   * as defined.
   *
   * In a real driver, the data to be written would be passed to the device
   * driver. That may involve interrupts.
   */

  if ( rw_args ) {
    printk("  %d to write\n", rw_args->count);
    printk("  buffer: %s\n", rw_args->buffer);
    rw_args->bytes_moved = rw_args->count;
  } else {
    printk("  nothing to write\n");
    rw_args->bytes_moved = 0;
  }

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver userdrv_control(
  rtems_device_major_number  major RTEMS_UNUSED,
  rtems_device_minor_number  minor RTEMS_UNUSED,
  void                      *pargp RTEMS_UNUSED
)
{
  rtems_libio_ioctl_args_t *ioctl_args = (rtems_libio_ioctl_args_t *) pargp;
  rtems_status_code         retval     = RTEMS_SUCCESSFUL;

  printk("  userdrv_control\n");

  /**
   * @internal
   *
   * In a more realistic driver, the ioctl entry would validate the minor
   * number passed in as representing a valid device instance. Then it would
   * valid the requested ioctl operation number as valid.  There is
   * likely more error checking on the arguments passed in. But it should
   * be safe to trust the file system framework to pass the @a argp buffer
   * as defined.
   */

  switch (ioctl_args->command) {
    case USERDRV_IOCTL_DO_SOMETHING:
      printk( "  Buffer: %s\n", (const char *) ioctl_args->buffer);
      ioctl_args->ioctl_return = 0;
      break;
    default:
      ioctl_args->ioctl_return = 0;
      retval = RTEMS_INVALID_NUMBER;
  } 

  return retval;
}
