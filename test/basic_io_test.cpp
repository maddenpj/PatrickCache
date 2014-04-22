#include "cache.h"

std::string option_to_string(Patrick::optional<std::string> opt) {
  if( opt )
    return *opt;
  else
    return std::string("");
}

std::string status_to_string(Patrick::Status status) {
  switch (status) {
    case Patrick::Status::STORED:
      return "STORED";
    case Patrick::Status::ERROR:
      return "ERROR";
    case Patrick::Status::DELETED:
      return "DELETED";
    default:
      return "Unknown Status";
  }
}

int main()
{
  Patrick::Cache c;

  auto status = c.set("foo", "bar");
  std::cout << "set foo bar" << std::endl << status_to_string(status) << std::endl;

  std::cout << "get foo" << std::endl << option_to_string(c.get("foo")) << std::endl;

  std::cout << "get something_undefined" << std::endl << option_to_string(c.get("something_undefined")) << std::endl;

  status = c.add("counter", "1");
  std::cout << "add counter 1" << std::endl << status_to_string(status) << std::endl;

  auto current = c.increment("counter");
  std::cout << "increment counter" << std::endl << std::get<1>(current) << std::endl;

  status = c.add("counter", "9");
  std::cout << "add counter 9" << std::endl << status_to_string(status) << std::endl;

  status = c.deleteElem("counter");
  std::cout << "delete counter" << std::endl << status_to_string(status) << std::endl;

  status = c.add("counter", "9");
  std::cout << "add counter 9" << std::endl << status_to_string(status) << std::endl;

  c.decrement("counter", "2");
  std::cout << "decrement counter 2" << std::endl << option_to_string(c.get("counter")) << std::endl;
}
