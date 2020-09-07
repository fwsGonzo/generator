#ifndef LIBRARY_CONFIG_HPP
#define LIBRARY_CONFIG_HPP

#include <map>
#include <sstream>
#include <string>

namespace library
{
class Config
{
public:
    Config() {}
    Config(const std::string& file);
    bool load(const std::string& file);

    // std::string
    std::string get(const std::string& var, const std::string& def)
    {
        // not found in hashmap -> return default
        if (kv.find(var) == kv.end()) return def;
        // return result
        return kv[var];
    }
    // const char*
    std::string get(const std::string& var, const char* def)
    {
        // not found in hashmap -> return default
        if (kv.find(var) == kv.end()) return def;
        // return result
        return kv[var];
    }
    // algebraic
    template <typename T>
    T get(const std::string& var, const T def)
    {
        // if variable is not found in hashmap, return default
        if (kv.find(var) == kv.end()) return def;
        // initialize result to default value
        T result;
        std::stringstream convert(kv[var]);
        // if the conversion fails, return default
        if (!(convert >> result)) return def;
        // return converted type T
        return result;
    }

    void set(const std::string& key, const std::string& value)
    {
        if (kv.find(key) == kv.end()) { kv.insert(std::make_pair(key, value)); }
        else
            kv[key] = value;
    }

    template <typename T>
    void set(const std::string& key, T val)
    {
        set(key, std::to_string(val));
    }

private:
    std::map<std::string, std::string> kv;
};
} // namespace library

#endif
