#include "lzo.hpp"

#include <lzo/lzo1x.h>
#include <lzo/lzo2a.h>
#include <cstdlib>
#include <string>

bool LZO::initialized = false;

// constructor for LZO object
LZO::LZO()
{
	// initialize lzo2
	if (this->initialized == false)
	{
		if (lzo_init() != LZO_E_OK)
			throw std::string("LZO::LZO() Failed to initialize lzo2");
	}
}

bool LZO::init(int bufferlen)
{
	// allocate compressor working memory
	lzo_workmem = new lzo_byte[LZO2A_999_MEM_COMPRESS];
	
	// allocate compression buffer (in bytes)
	compression_buffer = new lzo_byte[bufferlen];
	
	// remember the buffer size
	compression_length = bufferlen;
	
	return compression_buffer != nullptr;
}

LZO::~LZO()
{
	// deallocate compression buffer
	delete[] compression_buffer;
	// free working memory (for compression)
	delete[] lzo_workmem;
}

lzo_bytep LZO::getData()
{
	return compression_buffer;
}

lzo_uint LZO::getDataLength()
{
	return compression_length;
}

bool LZO::compress1x(lzo_bytep data, int datalen)
{
	// compress n (to n + nlen) into compression_buffer
	// compression_length is the resulting compressed size
	return lzo1x_1_15_compress(
		data, 
		datalen, 
		compression_buffer, 
		&compression_length, 
		lzo_workmem
	) == LZO_E_OK;
}
bool LZO::optimize1x(lzo_bytep data, int datalen)
{
	// optimizes a 1x compressed block
	return lzo1x_optimize(
		data, 
		datalen, 
		compression_buffer, 
		&compression_length, 
		lzo_workmem
	) == LZO_E_OK;
}
bool LZO::decompress1x(lzo_bytep data, int datalen)
{
	// decompresses lzo1x compressed data
	// compression_length is the resulting uncompressed size
	return lzo1x_decompress(
		data, 
		datalen, 
		compression_buffer, 
		&compression_length, 
		nullptr
	) == LZO_E_OK;
}

bool LZO::compress2a(lzo_bytep data, int datalen)
{
	// compress n (to n + nlen) into compression_buffer
	// compression_length is the resulting compressed size
	return lzo2a_999_compress(
		data, 
		datalen, 
		compression_buffer, 
		&compression_length, 
		lzo_workmem
	) == LZO_E_OK;
}
bool LZO::decompress2a(lzo_bytep data, int datalen)
{
	// decompresses data into compression_buffer
	// compression_length is the resulting uncompressed size
	return lzo2a_decompress(
		data, 
		datalen, 
		compression_buffer, 
		&compression_length, 
		nullptr
	) == LZO_E_OK;
}
