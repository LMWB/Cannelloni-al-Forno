#include "utils.h"

void myprintf(const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	UART_SEND_TERMINAL((uint8_t *) buffer, len);
}

bool is_over_timeout(uint32_t tick_start, uint32_t timeout_in_ms) {
	uint32_t tick_end = GET_TICK();
	if (tick_end - tick_start > timeout_in_ms) {
		return 1;
	}
	return 0;
}

void refresh_watchdog(void) {
	if (REFRESH_WATCHDOG() != DEVICE_OK) {
		myprintf("Watchdog refresh failed\n");
	}
}

uint8_t get_percentage_from_range(uint32_t min_value, uint32_t max_value, uint32_t current_value) {
	if (max_value == min_value) {
		return 0; // Avoid division by zero
	}
	if (current_value < min_value) {
		return 0;
	}
	if (current_value > max_value) {
		return 100;
	}
	uint8_t percentage = ((current_value - min_value) * 100) / (max_value - min_value);
	return percentage;
}

char *strstr_with_zero(const char *haystack, const char *needle, size_t limit) {
	size_t needle_len = strlen(needle);
	if (needle_len == 0) {
		return (char *) haystack;
	}

	for (size_t i = 0; i <= limit - needle_len; i++) {
		if (strncmp(&haystack[i], needle, needle_len) == 0) {
			return (char *) &haystack[i];
		}
	}

	return NULL;
}
