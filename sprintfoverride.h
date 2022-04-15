#ifndef SPRINTFOVERRIDE_H
#define	SPRINTFOVERRIDE_H

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "hal_io.h"
#include "hal_rtos.h"
#include "gm_SystemPinMap.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cppinit.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "e54manageadc.h"

int sprintu(char *s, unsigned u);
int sprintf(char *s, const char *fmt, ...);

#endif
