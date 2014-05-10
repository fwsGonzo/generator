#ifndef SOUND_HPP
#define SOUND_HPP

#include <library/math/vector.hpp>
#include <memory>
#include <string>

namespace library
{
	class SoundHandle;
	
	class Sound
	{
	private:
		typedef std::shared_ptr<SoundHandle> soundHandle;
		
		soundHandle handle;
		
		// initialization of sound library
		static int   initialized;
		// master volume for samples [0..1]
		static float masterVolume;
		
	public:
		Sound() {}
		Sound(std::string fname);
		Sound(std::string fname, int samples);
		
		Sound& operator= (const Sound& sound);
		
		void load(std::string filename, int samples);
		void setVolume(float vol);
		
		void play(vec3);
		void play();
		
		static void setMasterVolume(float vol);
		
		static const float MAX_PAN_DIST;
		static const float MAX_VOL_DIST;
	};
}

#endif
