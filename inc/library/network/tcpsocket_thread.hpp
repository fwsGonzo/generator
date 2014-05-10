#include <library/network/tcpsocket.hpp>

#include <thread>
#include <mutex>

namespace library
{
	class TcpSocketThread
	{
		static const int DEFAULT_RETRIES = 3;
		
		typedef bool (*pollingFunc)();
		
		bool persistent;
		int retries;
		std::thread thread;
		std::mutex  mtx;
		
		TcpSocket   socket;
		pollingFunc pollingFunction;
	};
	
	class TcpClientThread : public TcpSocketThread
	{
		std::string hostname;
		int         port;
		
		
	};
	
	class TcpServerThread : public TcpSocketThread
	{
		std::string interface;
		int         port;
		
	};
	
}
