#include "../include/logging.h"
#include <string>
#include <time.h>
#include <stdarg.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif

const char* k_log_file = "unkown source file";
static std::string log_file = "";

void create_logging_file()
{
	time_t rawtime;
	time(&rawtime);
	struct tm* tm = localtime(&rawtime);

	std::stringstream ssFile;
    ssFile << "kyrinSystemCheckClient_"
           << (tm->tm_year + 1900)
           << (tm->tm_mon < 9? "0":"")
           << (tm->tm_mon + 1)
           << (tm->tm_mday < 9? "0":"")
           << tm->tm_mday << ".txt";

	if(!log_file.empty())
	{
		if(ssFile.str() == log_file)
		{
			return;
		}
	}
	log_file = ssFile.str();

	FILE* fd = fopen(ssFile.str().c_str(),"a+");
	if(fd == nullptr)
		return;

	fclose(fd);

	freopen(ssFile.str().c_str(),"a+",stderr);
}

std::string String_Format(const char * format, ...) {
	va_list args;
	va_start(args, format);

	char buffer[4096] = {0};
	const int kBufferSize = sizeof(buffer)/sizeof(buffer[0]);
	const int size = vsnprintf(buffer, kBufferSize, format, args);
	va_end(args);

	if (size < 0 || size >= kBufferSize) {
		return std::string("<formatting error or buffer exceeded>");
	} else {
		return std::string(buffer, size);
	}
}

std::string FormatFileLocation(const char* file, int line) {
	const char* const file_name = file == nullptr ? k_log_file : file;

	if (line < 0) {
		return String_Format("%s:", file_name).c_str();
	}
	return String_Format("[%s:%d]", file_name, line).c_str();
}

Logging::Logging(LogSeverity severity, const char* file, int line)
	: severity_(severity)
{
	create_logging_file();

	const char* const marker =
		severity == LOG_INFO ?    "[  INFO ]" :
		severity == LOG_WARNING ? "[WARNING]" :
		severity == LOG_ERROR ?   "[ ERROR ]" : "[ FATAL ]";

	//PID [INFO] [HH:MM:SS]
	time_t rawtime;
	time(&rawtime);

	struct tm* tm = localtime(&rawtime);

	GetStream()
			<< "["
		   #ifndef _MSC_VER
			<< getpid()
		   #else
			<< GetCurrentProcessId()
		   #endif
			<<"] "
		   << marker
		   << " ["
		   << (tm->tm_hour >= 10 ?  "": "0" )
		   << tm->tm_hour
		   << ":"
		   << (tm->tm_min >= 10 ?  "": "0" )
		   << tm->tm_min
		   << ":"
		   << (tm->tm_sec >= 10 ?  "": "0" )
		   << tm->tm_sec
		   <<"] "
		  << FormatFileLocation(file, line).c_str();
}

Logging::~Logging()
{
	GetStream() << ::std::endl;
	std::cerr << stream_.str();
	if(severity_ == LOG_FATAL){
		fflush(stderr);
	}
}
