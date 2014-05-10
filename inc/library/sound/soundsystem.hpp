#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

namespace library
{
	class SoundSystem
	{
	private:
		static int references;
		
		static void increase();
		static void decrease();
		
		friend class Sound;
		friend class Stream;
		friend class SoundHandle;
	};
	
	
}

#endif // SOUNDSYSTEM_HPP
