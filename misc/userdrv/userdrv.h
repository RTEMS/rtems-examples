/**
 * @file
 *
 * @brief RTEMS /dev/userdrv Device Driver
 * 
 * This include file defines the interface to the RTEMS /dev/userdrv
 * device driver. This driver is intended to be used from section 2
 * system calls like open(), read(), etc. It does not support being used
 * from C Standard Library calls like fopen(), etc.
 *
 * The major number passed to the initialization method indicates the
 * slot in the RTEMS Device Driver Table. This can vary based upon the
 * application configuration. The same major number should be passed into
 * every invocation.
 *
 * The minor number indicates the device instance and is interpreted by
 * each device driver in a driver specific manner. It is common to simply
 * be an integer that indicates the instance number.
 *
 * The pargp or pointer to argument block parameter contents vary based
 * on the device driver entry. See the specific driver entry point for
 * details.
 *
 * The mapping between device drivers and the RTEMS file oriented
 * system calls is via the file libcsupport/src/sup_fs_deviceio.c.
 * For read, write, and ioctl, on error, the RTEMS Classic API status
 * will be mapped into a POSIX errno value and returned to the user
 * via the wrapper method in sup_fs_deviceio.c.
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


#ifndef _USERDRV_H
#define _USERDRV_H

#include <rtems/io.h>

/**
 *  @defgroup user_driver Example User Device Driver
 *
 *  @ingroup RTEMSDeviceDrivers
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This defines the device driver entries which go into the
 * Device Driver Table configured by the user. this 
 *
 * @code
 *   #define CONFIGURE_APPLICATION_EXTRA_DRIVERS \
 *      USERDRV_DRIVER_TABLE_ENTRY
 * @endcode
 */
#define USERDRV_DRIVER_TABLE_ENTRY \
  { userdrv_initialize, userdrv_open, userdrv_close, userdrv_read, \
    userdrv_write, userdrv_control }

/**
 * @brief User Driver Specific IOCTL
 */
#define USERDRV_IOCTL_DO_SOMETHING 0x320001

/**
 * @brief User Driver Initialize
 *
 * This routine is the user device driver init routine. It initializes
 * the devices and registers the device names for subsequent use. In this
 * specific driver, the device name /dev/userdrv is registered for
 * subsequent use.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to open parameter block
 *
 * The calling framework for driver initialization always passes
 * NULL for @a pargp.
 *
 * @return This method returns RTEMS_SUCCESSFUL on success and calls
 *         @a rtems_fatal_error otherwise.
 */
rtems_device_driver userdrv_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

/**
 * @brief User Driver Open
 *
 * This routine is the user device driver open routine. It may need
 * to check the access mode and manage if multiple opens are allowed
 * concurrently.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to open parameter block
 *
 * @a pargp points to a data structure of type @a rtems_libio_open_close_args_t
 * which is filled in as follows:
 *
 *   - iop - Pointer to an @a rtems_libio_t instance. This is the internal
 *     file management structure.
 *   - flags - libio style read/write access flags.
 *   - mode - access mode per POSIX @a mode_t
 *
 * @return This method returns RTEMS_SUCCESSFUL on success.
 */
rtems_device_driver userdrv_open(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

/**
 * @brief User Driver Close
 *
 * This routine is the user device driver close routine.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to close parameter block
 *
 * @a pargp points to a data structure of type @a rtems_libio_open_close_args_t
 * which is filled in as follows:
 *
 *   - iop - Pointer to an @a rtems_libio_t instance. This is the internal
 *     file management structure.
 *   - flags - is 0.
 *   - mode - is 0.
 *
 * @return This method returns RTEMS_SUCCESSFUL on success.
 */
rtems_device_driver userdrv_close(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

/**
 * @brief User Driver Read
 *
 * This routine is the user device driver read routine.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to open parameter block
 *
 * @a pargp points to a data structure of type @a rtems_libio_rw_args_t
 * which is filled in as follows:
 *
 *   - iop - Pointer to an @a rtems_libio_t instance. This is the internal
 *     file management structure.
 *   - offset - the file offset to read from.
 *   - buffer - the user buffer to read into.
 *   - count - the maximum number of bytes to read.
 *   - flags - libio style read/write access flags.
 *   - bytes_moved - set to 0.
 *
 * @return This method returns RTEMS_SUCCESSFUL on success. Additionally
 *    the @a bytes_moved field of the  @a rtems_libio_rw_args_t structure
 *    will have the number of bytes actually returned. On error, the
 *    appropriate Classic API status code is returned.
 */
rtems_device_driver userdrv_read(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

/**
 * @brief User Driver Write
 *
 * This routine is the user device driver write routine.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to open parameter block
 *
 * @a pargp points to a data structure of type @a rtems_libio_rw_args_t
 * which is filled in as follows:
 *
 *   - iop - Pointer to an @a rtems_libio_t instance. This is the internal
 *     file management structure.
 *   - buffer - user buffer to write from.
 *   - offset - the file offset to write to.
 *   - count - the maximum number of bytes to write.
 *   - flags - libio style read/write access flags.
 *   - bytes_moved - set to 0.
 *
 * @return This method returns RTEMS_SUCCESSFUL on success. Additionally
 *    the @a bytes_moved field of the  @a rtems_libio_rw_args_t structure
 *    will have the number of bytes actually written. On error, the
 *    appropriate Classic API status code is returned.
 */
rtems_device_driver userdrv_write(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

/**
 * @brief User Driver IO Control
 *
 * This routine is the user device driver IO control routine.
 *
 * @param[in] major The device major number
 * @param[in] minor The device minor number
 * @param[in] pargp The pointer to open parameter block
 *
 * @a pargp points to a data structure of type @a rtems_libio_ioctl_args_t
 * which is filled in as follows:
 *
 *   - iop - Pointer to an @a rtems_libio_t instance. This is the internal
 *     file management structure.
 *   - command - the ioctl command to execute.
 *   - buffer - user buffer to support the operation.
 *   - ioctl_return - undefined.
 *
 * @return This method returns RTEMS_SUCCESSFUL on success. Additionally
 *    the @a ioctl_return field of the  @a rtems_libio_ioctl_args_t structure
 *    will have the POSIX style return code to return to the user. On error,
 *    the appropriate Classic API status code is returned.
 */
rtems_device_driver userdrv_control(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *pargp
);

#ifdef __cplusplus
}
#endif
/**@}*/

#endif
/* end of include file */
