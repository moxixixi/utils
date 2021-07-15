#ifndef LOGGING_H
#define LOGGING_H
#include <sstream>
#include <iostream>
#include "DefUtil.h"

enum LogSeverity{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
};

class Logging
{
public:
	Logging(LogSeverity severity, const char* file, int line);
	~Logging();

	std::ostream& GetStream() { return stream_; }

CMM_NO_COPYABLE(Logging)

private:
	const LogSeverity severity_;
	std::ostringstream stream_;
};

#ifdef _CLIENT_NO_LOG_

#define LOG_(severity) std::cout

#else

#define LOG_(severity)				\
	Logging(LOG_##severity,			\
	__FILE__, __LINE__).GetStream()

#endif

//////////////////////////////////////////////////
/// FunctionLogger
///
class FunctionLogger
{
public:
	FunctionLogger(const std::string& func):_func(func)
	{
		LOG_(INFO) << "Function "<< func <<" started ================>>>";
	};
	~FunctionLogger()
	{
		LOG_(INFO) << "Function "<< _func <<" ended ================<<<";
	};
private:
	FunctionLogger();
	std::string _func;
};

#ifndef _CLIENT_NO_LOG_
#define LOGFUNC(x) FunctionLogger _funcLogger(x)
#else
#define LOGFUNC(x)
#endif

#endif // LOGGING_H
