/*
 * Copyright (c) 2020 Vijay Kumar Banerjee <vijay@rtems.org>.
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <stdlib.h>
#include <sysexits.h>

#include <rtems.h>
#include <rtems/bsd/bsd.h>
#include <bsp/i2c.h>
#include <rtems/console.h>
#include <rtems/shell.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include <fbdev.h>
#include "lvgl/lvgl.h"
#include <lv_drivers/indev/evdev.h>

#include "rtems_logo.h"

#define PRIO_SHELL      150
#define STACK_SIZE_SHELL    (64 * 1024)
#define PRIO_MOUSE          (RTEMS_MAXIMUM_PRIORITY - 10)

LV_IMG_DECLARE(rtems_logo);

typedef struct evdev_message {
    int fd;
    char device[256];
} evdev_message;

static rtems_id eid, emid;
static volatile bool kill_evtask, evtask_active;

static void*
tick_thread (void *arg)
{
    (void)arg;

    while(1) {
        lv_tick_inc(1);
        lv_task_handler();
        usleep(1000);
    }
}

static void
evdev_input_task(rtems_task_argument arg)
{
    rtems_status_code sc;
    size_t size;
    int fd = -1;
    pthread_t thread_id;

    evdev_message msg;

    kill_evtask = false;
    evtask_active = true;

    while(!kill_evtask){
        if (access(EVDEV_NAME, F_OK) != -1){
            evdev_init();
            break;
        }
    }

    pthread_create(&thread_id, NULL, tick_thread, NULL);
    evtask_active = false;
    rtems_task_delete(RTEMS_SELF);
}

void
libbsdhelper_start_shell(rtems_task_priority prio)
{
    rtems_status_code sc = rtems_shell_init(
        "SHLL",
        STACK_SIZE_SHELL,
        prio,
        CONSOLE_DEVICE_NAME,
        false,
        true,
        NULL
    );
    assert(sc == RTEMS_SUCCESSFUL);
}

void lv_ex_img_1(void){
    lv_obj_t* img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &rtems_logo);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void
event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
        lv_obj_del(obj);
        lv_ex_img_1();
    }
}

void lv_ex_btn_1(void)
{
    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 0);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Click me");
}

static void
Init(rtems_task_argument arg)
{
    rtems_status_code sc;
    int exit_code;
    (void)arg;

    sc = rtems_task_create(
    rtems_build_name('E', 'V', 'D', 'M'),
    PRIO_MOUSE,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_FLOATING_POINT,
    &eid
    );
    assert(sc == RTEMS_SUCCESSFUL);

    sc = rtems_task_start(eid, evdev_input_task, 0);
    assert(sc == RTEMS_SUCCESSFUL);

    sc = rtems_bsd_initialize();
    assert(sc == RTEMS_SUCCESSFUL);

    lv_init();
    static lv_color_t buf[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10];
    static lv_disp_buf_t disp_buf;
    fbdev_init();

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX / 10);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv);
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);

    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(cursor_obj, LV_SYMBOL_PLAY);
    lv_indev_set_cursor(mouse_indev, cursor_obj);

    lv_ex_btn_1();

    lv_tick_inc(5);
    lv_task_handler();

    libbsdhelper_start_shell(PRIO_SHELL);
    kill_evtask = true;
    while (evtask_active) {
      rtems_task_wake_after(RTEMS_MILLISECONDS_TO_TICKS(10));
    }

    exit(0);
}
/*
 * Configure LibBSD.
 */
#define RTEMS_BSD_CONFIG_BSP_CONFIG
#define RTEMS_BSD_CONFIG_TERMIOS_KQUEUE_AND_POLL
#define RTEMS_BSD_CONFIG_INIT

#include <machine/rtems-bsd-config.h>

/*
 * Configure RTEMS.
 */
#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 32

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1

#define CONFIGURE_INIT_TASK_STACK_SIZE (64*1024)
#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_BDBUF_BUFFER_MAX_SIZE (32 * 1024)
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS 4
#define CONFIGURE_BDBUF_CACHE_MEMORY_SIZE (1 * 1024 * 1024)
#define CONFIGURE_BDBUF_READ_AHEAD_TASK_PRIORITY 97
#define CONFIGURE_SWAPOUT_TASK_PRIORITY 97

//#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT

#include <rtems/confdefs.h>
#include <bsp/nexus-devices.h>

SYSINIT_DRIVER_REFERENCE(ukbd, uhub);
SYSINIT_DRIVER_REFERENCE(ums, uhub);

#include <rtems/netcmds-config.h>
#include <bsp/irq-info.h>
#define CONFIGURE_SHELL_COMMANDS_INIT

#define CONFIGURE_SHELL_USER_COMMANDS \
  &bsp_interrupt_shell_command, \
  &rtems_shell_ARP_Command, \
  &rtems_shell_I2C_Command

#define CONFIGURE_SHELL_COMMANDS_ALL

#include <rtems/shellconfig.h>
