#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H


#include <stdlib.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg function
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "hardwareGlobal.h"

#define BREAK_ON_ERROR(function) do { \
    int error = (function); \
    if(error == 1) return error; \
} while(0)


void myprintf(const char *fmt, ...);

bool is_over_timeout(uint32_t tick_start, uint32_t timeout_in_ms);

void refresh_watchdog();

uint8_t get_percentage_from_range(uint32_t min_value, uint32_t max_value, uint32_t current_value);

char *strstr_with_zero(const char *haystack, const char *needle, size_t limit);

#endif /* COMMON_UTILS_H */