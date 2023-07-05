#pragma once
#include <string>

class LoggerClass
{
	static inline std::string log;

public:
	static inline void Log(std::string txt)
	{
		log.append(txt);
	}

	static inline void ClearLog()
	{
		log.clear();
	}

	static inline std::string& GetContent()
	{
		return log;
	}
};