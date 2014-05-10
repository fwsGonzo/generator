#ifndef HOSTINFO_HPP
#define HOSTINFO_HPP

#ifdef __linux__
	#include <netdb.h>
	//extern int* h_errno;
#endif

#include <string>

struct hostent;

namespace library
{
	class HostInfo
	{
	private:
		#ifdef __linux__
			char searchHostDB;     // search the host database flag
		#endif
		
		struct hostent* hostPtr;    // Entry within the host address database
		
	public:
		typedef enum
		{
			HOSTNAME,
			ADDRESS
		} hosttype_t;
		
		bool good;
		
		// retrieves local hostname
		HostInfo();
		// retrieves hostname or address
		HostInfo(std::string& hostname, HostInfo::hosttype_t type);
	 
		~HostInfo()
		{
			#ifdef __linux__
				endhostent();
			#endif
		}
		
		#ifdef __linux__
			// Retrieves the next host entry in the database
			char getNextHost();

			// Opens the host entry database
			void openHostDb()
			{
				endhostent();
				searchHostDB = 1;
				sethostent(1);
			}
		#endif
		
		char* getHostIPAddress();
		char* getHostName();
	};
	
}

#endif
