#ifndef CACHE_H
#define CACHE_H

#include <boost/optional.hpp>
#include <unordered_map>

namespace Patrick {
  template <class T> using optional = boost::optional<T>;
  typedef std::string String;

  struct Key 
  {
    optional<String> value;
    optional<int> expiration;

    Element() {}

    Element(const String& value_, const optional<int>& expiration_)
      : value(value_), expiration(expiration_)
    {
    }
  };
  
  enum class Status { STORED, ERROR };

  class Cache
  {
    private:
      const int MAX_SIZE = 16 * 1048576;
      const int KEY_SIZE = 64;
      const int VALUE_SIZE = 255;
      std::unordered_map<String, int> accessCount; 
      std::unordered_map<String, Element> storage;

    public:
      Cache()
        : storage()
      {
      }

      optional<String> get(const String& key)
      {
        if (storage.find(key) == storage.end()) {
          return optional<String>();
        } else {
          auto val = storage.at(key);
          accessCount[key]++;
          return val.value;
        }
      }

      Status set(const String& key, const String& value, const optional<int>& expires=optional<int>())
      {
        Element e(value, expires);
        try {
          storage[key] = e;
          accessCount[key] = 0;
        } catch(...) {
          auto eptr = std::current_exception(); // Useful for logging later
          return Status::ERROR;
        }
        return Status::STORED;
      }
  };

}

#endif
