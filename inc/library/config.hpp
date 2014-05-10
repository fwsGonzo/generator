#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <string>
#include <sstream>

namespace library
{
	class Config
	{
		std::map<std::string, std::string> kv;
		
	public:
		Config();
		Config(std::string);
		bool load(std::string file);
		
		inline const char* get(std::string var, const char* def)
		{
			// if variable is not found in hashmap, return default
			if (kv.find(var) == kv.end()) return def;
			// return C-string
			return kv[var].c_str();
		}
		
		template <typename ValueType>
		inline ValueType get(std::string var, const ValueType def)
		{
			// if variable is not found in hashmap, return default
			if (kv.find(var) == kv.end()) return def;
			// initialize result to default value
			ValueType result;
			std::stringstream convert(kv[var]);
			// if the conversion fails, return default
			if (!(convert >> result)) return def;
			// return converted type T
			return result;
		}
		
		// redirect for c-strings
		template <typename T>
		inline T get(const char* var, T def)
		{
			return get(std::string(var), def);
		}
		
		template <typename T>
		inline void set(std::string key, T value)
		{
			if (kv.find(key) == kv.end()) 
			{
				kv.insert(
					std::pair<std::string,std::string>(key, value)
				);
			}
			else kv[key] = value;
		}
		
	};
	// default config-file object
	extern Config config;
}

#endif
