//log.h

#ifndef CSIMPLOG_H
#define CSIMPLOG_H

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <string>

class csimpLog : public std::ofstream
{
public:
	csimpLog(); //on successful creation is_open() is true
	~csimpLog();

	void logSDLerror(void);
	void logtime(void);
};

extern csimpLog csimplog;

#endif
