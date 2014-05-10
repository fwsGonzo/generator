#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <library/network/hostaddr.hpp>

namespace library
{
	class Socket
	{
	public:
		typedef enum
		{
			STAGE_INIT,
			STAGE_OPTION,
			
			STAGE_RESOLVE,
			STAGE_CONNECT,
			STAGE_DISCONNECT,
			
			STAGE_BIND,
			STAGE_LISTEN,
			STAGE_ACCEPT,
			
			STAGE_POLL,
			
			STAGE_READ,
			STAGE_WRITE
			
		} socket_stage_t;
		
	protected:
		// set socket descriptor
		void setSocketId(int socketFd) { socketId = socketFd; }
		// sets a socket option
		bool socketOption(int option, int value);
		// blocking / non-blocking socket
		bool setSocketBlocking(bool block);
		
		int socketId;     // socket descriptor
		void* clientAddr; // 
		bool blocking;    // blocking flag
		int bindFlag;     // binding flag
		void* userData;   // user-provided data pointer
		
		// error-related fields
		int error;
		std::string errorString;
		bool handleErrors(socket_stage_t, bool ignoreBlock);
		
		// limits / constants
		static const int MAX_RECV_LEN = 8096;
		static const int MAX_MSG_LEN  = 1024;
		
	public:
		// initializes sockets, if needed, and sets blocking to true
		Socket(void* userdata);
		virtual ~Socket();
		
		// resolve address or hostname
		typedef enum
		{
			HOSTNAME,
			ADDRESS
		} hosttype_t;
		// example:
		//     Socket::resolve("www.vg.no", Socket::HOSTNAME)
		static std::string resolve(std::string name, hosttype_t type);
		
		// user-provided data
		void* getUserData() { return this->userData; }
		
		// socket options ON/OFF
		bool setReuseAddr(int);
		bool setKeepAlive(int);
		bool setSendBufSize(int);
		bool setReceiveBufSize(int);
		
		// retrieve socket option settings
		int  getReuseAddr();
		int  getKeepAlive();
		int  getSendBufSize();
		int  getReceiveBufSize();
		
		// events
		typedef void (*SocketEvent_Error)(Socket*, socket_stage_t, int error, std::string& errorString);
		SocketEvent_Error      onError;
		
		// sadface functions :-(
		int          getError()       { return this->error; }
		std::string& getErrorString() { return this->errorString; }
		std::string  stageToString(socket_stage_t);
		
	private:
		static bool init;
	};
	
}

#endif
