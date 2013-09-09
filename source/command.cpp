#include "command.hpp"

#include <sstream>

namespace generator
{
	std::vector<std::string> split(const std::string& cmd, const std::string& delim)
	{
		std::vector<std::string> vec;
		if (delim.length() == 0) return vec;
		
		size_t start = 0, end = 0;
		
		while (end != std::string::npos)
		{
			end = cmd.find(delim, start);
			
			// If at end, use length=maxLength.  Else use length=end-start.
			vec.push_back(cmd.substr(start,
				(end == std::string::npos) ? std::string::npos : end - start));
			
			// If at end, use start=maxSize.  Else use start=end+delimiter.
			start = ( ( end > (std::string::npos - delim.size()) )
					?  std::string::npos  :  end + delim.size());
		}
		return vec;
	}
	
	Command::Command(std::string line)
	{
		this->arglist = split(line, " ");
	}
	
	int Command::getInt(int arg)
	{
		if (arg < 0 || arg >= args())
			throw std::string("Argument out of bounds");
		
		int result;
		std::stringstream convert(arglist[arg]);
		if (!(convert >> result))
			throw std::string("Not a number");
		
		return result;
	}
	
	std::string Command::getString(int arg)
	{
		if (arg < 0 || arg >= args())
			throw std::string("Argument out of bounds");
		
		return arglist[arg];
	}
}

