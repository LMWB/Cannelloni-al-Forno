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
