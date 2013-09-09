#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

namespace generator
{
	class Command
	{
	public:
		Command(std::string line);
		
		int args() const
		{
			return arglist.size();
		}
		
		int getInt(int arg);
		std::string getString(int arg);
		
	private:
		std::vector<std::string> arglist;
	};
}

#endif // COMMAND_HPP
