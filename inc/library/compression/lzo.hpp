#ifndef LZO_HPP
#define LZO_HPP

#include <lzo/lzoconf.h>

namespace library
{
	class LZO
	{
	private:
		static bool initialized;
		static const lzo_uint COMPR_STDLEN = 1024;
		
		lzo_uint  compression_length = COMPR_STDLEN;
		lzo_bytep compression_buffer;
		lzo_bytep lzo_workmem;
		
		
	public:
		LZO();
		~LZO();
		bool init(int bufferlen);
		
		bool compress1x(lzo_bytep data, int datalen);
		bool decompress1x(lzo_bytep data, int datalen);
		bool optimize1x(lzo_bytep data, int datalen);
		
		bool compress2a(lzo_bytep data, int datalen);
		bool decompress2a(lzo_bytep data, int datalen);
		
		lzo_uint  getDataLength();
		lzo_bytep getData();
	};
	
}

#endif
