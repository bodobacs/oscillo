//log.cpp
#include <SDL.h>
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include "csimpLog.h"

csimpLog csimplog;

csimpLog::csimpLog() : std::ofstream("log.txt", std::ofstream::out)
{
	if(std::ofstream::is_open())
	{
		logtime();
	}else{
		std::cout << "csimpLog initialization failed." << std::endl;
	}
}

csimpLog::~csimpLog()
{
	if(is_open())
	{
		(*this) << "Closing" << std::endl;
		close();
	}
}

void csimpLog::logSDLerror(void)
{
	(*this) << " SDL_Init Error: " << SDL_GetError() << std::endl;
}

void csimpLog::logtime(void)
{
	std::time_t timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	(*this) << std::ctime(&timenow) << std::endl;
}
