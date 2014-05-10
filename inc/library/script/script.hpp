#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
struct TCCState;

namespace library
{
	class Script
	{
	public:
		typedef void (*error_func)(void* opaque, const char* msg);
		
		Script();
		~Script();
		
		void addsymb(const std::string& symb, const void* func);
		void compile(const std::string& program);
		
		int execute(const std::string& function);
		int execute(const std::string& function, void* data);
		
		void setErrorFunc(error_func errfunc);
		
	private:
		TCCState* state;
		void*     memory;
	};
	
}

#endif // SCRIPT_HPP
