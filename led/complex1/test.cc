//
//

#include <bsp.h>
#if !defined(BSP_SMALL_MEMORY)

#include <stdio.h>
#include "ledServer.h"
#include "menu.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Examples::LEDServer *led;

void StartLED(void)
{
  led->start();
}

void StopLED(void)
{
  led->stop();
}

void SetPeriodOfLED(void)
{
  unsigned int onPeriod;
  unsigned int offPeriod;
  onPeriod = Test_menu_Get_int_bounded(
    "Length LED is ON (in milliseconds)",
    1,
    10000
  );
  offPeriod = Test_menu_Get_int_bounded(
    "Length LED is OFF (in milliseconds)",
    1,
    10000
  );
  led->setPeriod( onPeriod, offPeriod );
}

Menu_entry LEDMenu_entries[] = {
 { "Activate LED Server",            StartLED },
 { "Deactivate LED Server",          StopLED },
 { "Set Period of LED Server",       SetPeriodOfLED },
};

Test_menu LEDMenu = {
  "LED Server Control Menu",
  (sizeof(LEDMenu_entries) / sizeof(LEDMenu_entries[0])),
  LEDMenu_entries
};


/*
 *  Top level menu
 */

#define MAX_FILES_IN_MENU 60

char *FileNames[MAX_FILES_IN_MENU];
int  FileCount = 0;
bool FileNamesLoaded = false;

extern "C" {
  // strcasecmp is not 100% portable
  int Mystrcasecmp( const char *s1, const char *s2 )
  {
    while (*s1 != '\0' && tolower(*s1) == tolower(*s2)) {
      s1++;
      s2++;
    }
    return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
  }

  static int qsortcmp( const void *l, const void *r )
  {
    char **ld = (char **)l;
    char **rd = (char **)r;

    // fprintf( stderr, "-%s- -%s-\n", *ld, *rd );
    return Mystrcasecmp( *ld, *rd );
  }
};

int LoadDirectoryMenu(void)
{
  DIR *dir;
  struct dirent *d;

  if (FileNamesLoaded)
    return 0;
  FileNamesLoaded = true;

  /* initialize */
  dir = opendir(".");

  if (!dir) {
    fprintf( stderr, "Unable to open current directory\n" );
    return -1;
  }

  /* find all the .txt files */

  FileNames[ FileCount++ ] = strdup("Return to previous menu");

  for ( ; ;  ) {
    d = readdir(dir);
    if (!d)
      break;

    FileNames[ FileCount++ ] = strdup( d->d_name );

    /* fprintf( stderr, "%s\n", d->d_name ); */
    if ( FileCount >= MAX_FILES_IN_MENU ) {
      fprintf( stderr, "Too many files in current directory\n" );
      return -1;
    }
  }

  /* shutdown */
  closedir( dir );

  qsort( &FileNames[1], FileCount - 1, sizeof(char *), qsortcmp );

  return 0;
}

void LEDMenuFunction(void)
{
  Test_menu_run( &LEDMenu );
}

void statFile()
{
  int selected;
  struct stat statb;

  if ( LoadDirectoryMenu() )
    return;

  selected = Test_menu_Get_selection(
    "Select a File To stat()",
    FileCount,
    FileNames
  );
  if ( selected == 0 )
    return;

  fprintf( stderr,
     "You picked %d --> %s\n", selected, FileNames[selected] );

  if ( stat( FileNames[selected], &statb ) ) {
    fprintf( stderr, "Error stating file\n" );
    return;
  }

  fprintf( stderr, "Owner/Group: %d %d Permissions: 0%o\n"
                   "Size... in bytes: %ld  in blocks: %ld of %ld\n"
                   "Times of Last.. Access: %ld Modification %ld Change: %ld\n",
     statb.st_uid, statb.st_gid, statb.st_mode,
     (long) statb.st_size, (long) statb.st_blocks, (long) statb.st_blksize,
     (long) statb.st_atime, (long) statb.st_mtime, (long) statb.st_ctime );
}

#if defined(__rtems__)
#include <rtems.h>
#include <rtems/stackchk.h>
#include <rtems/cpuuse.h>

#include "periodic.h"

void SetNumberOfPeriodicThreads(void)
{
  unsigned int threads = Test_menu_Get_int_bounded(
    "How many periodic threads do you want to create",
    1,
    100
  );
  PeriodicThreads_Count = threads;
}

Menu_entry PeriodicThreadsMenu_entries[] = {
 { "Start Periodic Threads",         PeriodicThreads_Start},
 { "Stop Periodic Threads",          PeriodicThreads_Stop},
 { "Set Number Of Periodic Threads", SetNumberOfPeriodicThreads},
};

Test_menu PeriodicThreadsMenu = {
  "Periodic Threads Menu",
  (sizeof(PeriodicThreadsMenu_entries) /
          sizeof(PeriodicThreadsMenu_entries[0])),
  PeriodicThreadsMenu_entries
};

void PeriodicThreadsFunction(void)
{
  Test_menu_run( &PeriodicThreadsMenu );
}

#endif

Menu_entry MainMenu_entries[] = {
 { "LED Server Control Menu",        LEDMenuFunction },
 { "stat() a file",                  statFile },
#if defined(__rtems__)
 { "Periodic Threads Control Menu",  PeriodicThreadsFunction },
 { "Stack Usage Report",             rtems_stack_checker_report_usage },
 { "CPU Usage Report",               rtems_cpu_usage_report },
 { "Reset CPU Usage Statistics",     rtems_cpu_usage_reset },
 { "Period Usage Report",            rtems_rate_monotonic_report_statistics },
 { "Reset Period Usage Statistics",  rtems_rate_monotonic_reset_all_statistics},
#endif
};

Test_menu MainMenu = {
  "Main Menu",
  (sizeof(MainMenu_entries) / sizeof(MainMenu_entries[0])),
  MainMenu_entries
};

void doTest(void)
{
  LED_INIT();

  led = new Examples::LEDServer();

  Test_menu_run( &MainMenu );
#if defined(__rtems__)
  PeriodicThreads_Stop();
#endif
  delete led;
}

#endif
