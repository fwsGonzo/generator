#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

/**
 * Thread-safe logger class, logs to file & console/stdout (if enabled)
 * 
 * To open a logfile:
 * 	  library::logger.open("myproject.log");
 * 
 * To disable console/stdout ouputput:
 * logger.setOutputConsole(false);
 * 
 * To inform about initialization/progression:
 * logger << Log::INFO << "* Sound system initialized" << Log::ENDL;
 * 
 * To log an error:
 * logger << Log::ERR << "MyClass::myFunction(): Something bad happened: " << error_number << Log::ENDL;
 * 
 * WARNING:
 * Not using ENDL to finish a << stream will cause a mutex deadlock
 * 
**/

namespace library
{
	class Log
	{
		public:
			enum LogLevel
			{
				ENDL, INFO, WARN, ERR
			};
			
			Log();
			Log(std::string);
			~Log();
			void open(std::string);
			bool write(LogLevel, std::string);
			
			void setOutputConsole(bool console);
			
		private:
			bool autoLock;
			std::mutex synch;
			std::string log;
			std::ofstream file;
			
			bool outputConsole;
			
			friend Log& operator<< (Log& out, const LogLevel level);
			template <class T>
			friend Log& operator<< (Log& out, const T t);
	};
	
	// Log << T
	template <class T>
	Log& operator<< (Log& out, const T t)
	{
		std::stringstream ss;
		ss << t;
		out.log += ss.str();
		return out;
	}
	
	extern Log logger;
	
}

#endif
