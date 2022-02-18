/**
 * @file 
 *
 * @brief Doxygen Main Page for Example User Driver
 *
 * @mainpage
 *
 * This is an example RTEMS user driver which is accessed via POSIX system
 * calls such as open(2), read(2), etc. It is not intended to be used via
 * the corresponding Standard C Library calls such as fopen(3), fread(3), etc.
 *
 * The header file documents the interface and what is passed in from the
 * device driver file adapter.
 *
 * This is not a particularly exciting device driver. It just prints to
 * indicate which operations have been invoked, prints strings passed
 * via write(2), and returns string data via read(2).
 *
 * It is just a starting point for custom device drivers. It should also
 * be a good starting point for writing an RTEMS device driver which mimics
 * the interface provided by a FreeBSD or GNU/Linux device driver which
 * has a user level library making supporting calls to a device driver.
 *
 * It is not an example of a driver supporting termios, block access, or
 * network interfaces.
 */
