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

std::pair<Status, int> Cache::incrementInteger(const String& key, int other)
{
  if(storage.find(key) == storage.end()) {
    return std::make_pair(Status::ERROR, 0);
  } else {
    try {
      auto elem = storage[key];
      int value = std::stoi(elem.value);
      value += other;
      elem.value = std::to_string(value);
      storage[key] = elem;
      accessCount[key]++;
      return std::make_pair(Status::STORED, value);
    } catch(...) {
      auto eptr = std::current_exception(); // Useful for logging later
      return std::make_pair(Status::ERROR, 0);
    }
  }
}

std::pair<Status, int> Cache::incrementHelper(const String& key, const String& amount, std::function<int(int)> f)
{
  try {
    int amountInt = std::stoi(amount);
    return incrementInteger(key, f(amountInt));
  } catch (std::invalid_argument& e) {
    return std::make_pair(Status::ERROR, 0);
  } catch (std::out_of_range& e) {
    return std::make_pair(Status::ERROR, 0);
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

void Cache::expire(int seconds) {
  for (auto& x : storage) {
    if(x.second.expiration) {
      std::cout << x.first << std::endl;
      std::cout << *x.first.expiration << std::endl;
      *x.first.expiration--;
      std::cout << *x.first.expiration << std::endl;
    }
  }
}
