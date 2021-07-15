#pragma once

typedef int CmmBool;
#define CmmTrue 1
#define CmmFalse 0

#define CMM_NO_COPYABLE(classname)	\
	private:						\
	classname (const classname&);	\
	classname& operator = (const classname&);

#if defined(__linux__) || defined(__linux)
#define Q_OS_LINUX
#endif

//#define _CLIENT_NO_LOG_

#include "logging.h"
