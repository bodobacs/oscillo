/*log.h*/
#ifndef LOG_H
#define LOG_H

#include <cstdarg>
#include <cstdio>

void log_error(const char *format, ...);
void log_debug(const char *format, ...);

#endif

