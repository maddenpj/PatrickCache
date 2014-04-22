#include "cache.h"

using namespace Patrick;

optional<String> Cache::get(const String& key)
{
  if (storage.find(key) == storage.end()) {
    return optional<String>();
  } else {
    auto val = storage.at(key);
    accessCount[key]++;
    return val.value;
  }
}

Status Cache::set(const String& key, const String& value, const optional<int>& expires)
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

Status Cache::add(const String& key, const String& value, const optional<int>& expires)
{
  if(storage.find(key) != storage.end()) {
    return Status::ERROR;
  } else {
    return set(key, value, expires);
  }
}

Status Cache::incrementInteger(const String& key, int other)
{
  if(storage.find(key) == storage.end()) {
    return Status::ERROR;
  } else {
    try {
      auto elem = storage[key];
      int value = std::stoi(elem.value);
      value += other;
      elem.value = std::to_string(value);
      storage[key] = elem;
      accessCount[key]++;
    } catch(...) {
      auto eptr = std::current_exception(); // Useful for logging later
      return Status::ERROR;
    }
    return Status::STORED;
  }
}

Status Cache::incrementHelper(const String& key, const String& amount, std::function<int(int)> f)
{
  try {
    int amountInt = std::stoi(amount);
    return incrementInteger(key, f(amountInt));
  } catch (std::invalid_argument& e) {
    return Status::ERROR;
  } catch (std::out_of_range& e) {
    return Status::ERROR;
  }
}

Status Cache::deleteElem(const String& key)
{
  try {
    auto deleted = storage.erase(key);
    if (deleted > 0) {
      accessCount.erase(key);
      return Status::DELETED;
    } else {
      return Status::ERROR;
    }
  } catch(...) {
    auto eptr = std::current_exception(); // Useful for logging later
    return Status::ERROR;
  }
}
