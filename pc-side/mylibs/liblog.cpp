/*log.cpp*/
#include "liblog.h"

void log_error(const char *format, ...)
{
	va_list args;
	va_start (args, format);
	vfprintf (stderr, format, args);
	va_end (args);
}

void log_debug(const char *format, ...)
{
	va_list args;
	va_start (args, format);
	vfprintf (stdout, format, args);
	va_end (args);
}
