#include <bass.h>
#include <string>
#include "soundsystem.hpp"

namespace library
{
	
	class SoundHandle
	{
	public:
		enum HandleType
		{
			SAMPLE,
			STREAM
		};
		
		SoundHandle(DWORD handle, HandleType ht)
		{
			this->handle = handle;
			this->handleType = ht;
		}
		~SoundHandle()
		{
			if (this->handle)
			{
				switch (handleType)
				{
					case SAMPLE:
						BASS_SampleFree(this->handle);
						break;
					case STREAM:
						BASS_StreamFree(this->handle);
						break;
				}
				// a sound has been removed from the system, decrease reference counter
				SoundSystem::decrease();
			}
		}
		
		operator DWORD ()
		{
			return this->handle;
		}
		
	private:
		HandleType handleType;
		DWORD handle;
	};
	
}
