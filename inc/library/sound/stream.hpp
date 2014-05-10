#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <string>
#include <memory>

namespace library
{
	class SoundHandle;
	
	class Stream
	{
	private:
		typedef std::shared_ptr<SoundHandle> soundHandle;
		
		soundHandle handle;
		
		// crossfade related
		float  volume;
		double startTime;
		// master volume for streams [0..1]
		static float masterVolume;
		
	public:
		Stream();
		Stream(std::string);
		
		void load(std::string filename);
		
		bool play();
		void stop();
		
		// returns true if the stream is currently active
		bool isPlaying() const noexcept;
		
		// set volume to level (0..1) in time_ms (milliseconds)
		void setVolume(float vol);
		
		static void setMasterVolume(float vol);
		
		friend class StreamChannel;
	};
}

#endif
