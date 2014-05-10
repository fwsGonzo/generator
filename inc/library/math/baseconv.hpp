#ifndef BASECONV_HPP
#define BASECONV_HPP

#include <string>

namespace library
{
	class BaseConv
	{
	public:
		static std::string base32(unsigned int coordinate, int length);
		static std::string base64(unsigned int coordinate, int length);
	};
}

#endif
