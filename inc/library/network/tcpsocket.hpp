#include <library/network/socket.hpp>

namespace library
{
	
	class TcpSocket : public Socket
	{
	public:
		TcpSocket(void* userdata);
		~TcpSocket();
		
		typedef void (*SocketEvent_Connecting)(TcpSocket*, std::string& host, int port);
		typedef void (*SocketEvent_Connected)(TcpSocket*, std::string& host, int port);
		typedef void (*SocketEvent_Disconnect)(TcpSocket*);
		typedef void (*SocketEvent_Read)(TcpSocket*, std::string& data);
		typedef void (*SocketEvent_Listen)(TcpSocket*, std::string& addr, int port);
		typedef void (*SocketEvent_Accept)(TcpSocket*, int port, TcpSocket* newClient);
		
		SocketEvent_Connecting onConnect;
		SocketEvent_Connected  onConnected;
		SocketEvent_Disconnect onDisconnect;
		SocketEvent_Read       onDataReceived;
		SocketEvent_Listen     onListening;
		SocketEvent_Accept     onAccept;
		
		bool listening;
		bool connecting, connected;
		bool isConnecting() { return this->connecting; }
		bool isConnected() { return this->connected; }
		bool isListening() { return this->listening; }
		
		std::string getPeerName() { return this->peerName; }
		std::string getPeerAddr() { return this->peerAddr; }
		int         getPeerPort() { return this->peerPort; }
		
		// connect to server, port
		bool connect(std::string&, int port, HostInfo::hosttype_t);
		bool disconnect();
		
		bool bind(std::string& iface, int port);
		TcpSocket* accept(std::string&);
		bool listen(int numPorts = 5);
		
		// write line
		bool writeLine(std::string);
		
		// polling function / event handler
		void poll();
		
		// total received bytes
		int getRx() { return totalRx; }
		// total transmitted bytes
		int getTx() { return totalTx; }
		
	private:
		// creates a TCP_PROTO socket
		bool createTCPsocket();
		
		unsigned long totalRx, totalTx;
		
		// read and write queues
		std::string readq, sendq;
		// read buffer
		std::string buffer;
		// text parser
		void parseLine(std::string& text);
		
		// read and write strings of data
		int read(std::string&);
		int write(std::string&);
		bool disconnect(bool silent);
		
		std::string peerName;
		std::string peerAddr;
		int         peerPort;
		
		std::string localAddr;
		int         localPort;
	};
}
