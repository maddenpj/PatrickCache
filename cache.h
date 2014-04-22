#ifndef CACHE_H
#define CACHE_H

#include <boost/optional.hpp>
#include <unordered_map>
#include <string>

namespace Patrick {
  template <class T> using optional = boost::optional<T>;
  typedef std::string String;

  struct Element
  {
    String value;
    optional<int> expiration;

    Element() {}

    Element(const String& value_, const optional<int>& expiration_)
      : value(value_), expiration(expiration_)
    {
    }
  };
  
  enum class Status { STORED, ERROR, DELETED };

  class Cache
  {
    private:
      const long MAX_SIZE = 16 * 1048576;
      const int KEY_SIZE = 64;
      const int VALUE_SIZE = 255;
      long cacheSize;

      std::unordered_map<String, int> accessCount; 
      std::unordered_map<String, Element> storage;

    public:
      Cache()
        : storage(), accessCount()
      {
      }
      // TODO: Add helper constructors: existing maps, move constructors

      optional<String> get(const String& key);
      Status set(const String& key, const String& value, const optional<int>& expires=optional<int>());
      Status add(const String& key, const String& value, const optional<int>& expires=optional<int>());
      std::pair<Status, int> increment(const String& key, const String& amount="1")
      {
        return incrementHelper(key, amount, [](int x) -> int { return x; });
      }
      std::pair<Status, int> decrement(const String& key, const String& amount="1")
      {
        return incrementHelper(key, amount, [](int x) -> int { return -x; });
      }

      Status deleteElem(const String& key);

      void expire(int seconds=1);
      void garbageCollect();
      void cleanUp()
      {
        expire();
        garbageCollect();
      }

    private:
      std::pair<Status, int> incrementInteger(const String& key, int other);
      std::pair<Status, int> incrementHelper(const String& key, const String& amount, std::function<int(int)> f);

  };

}

#endif
