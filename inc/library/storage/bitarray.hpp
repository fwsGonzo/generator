#ifndef BITARRAY_HPP
#define BITARRAY_HPP

#include <cstdint>
#include <memory>

namespace library
{
	class BitArray
	{
	public:
		typedef uint32_t bitarray_t;
		
	private:
		static const bitarray_t SEG_BITS = sizeof(bitarray_t) * 8;
		static const bitarray_t SEG_SHR  = 5;
		
		int size;
		std::unique_ptr<bitarray_t[]> bits;
		
		inline bitarray_t& segment(int index) const
		{
			return bits[index >> SEG_SHR];
			
		}
		inline bitarray_t bitvalue(int index) const
		{
			return 1 << (index & (SEG_BITS-1));
		}
		bitarray_t segsize(int size) const;
		
	public:
		BitArray() : size(0) {}
		BitArray(int size);
		BitArray(const BitArray&);
		
		BitArray& operator= (const BitArray&);
		
		inline bool operator [] (int index) const
		{
			return (segment(index) & bitvalue(index)) != 0;
		}
		
		inline void set (int index)
		{
			segment(index) |= bitvalue(index);
		}
		
		void clear();
		
	};
}

#endif
