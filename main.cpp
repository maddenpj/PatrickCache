#include "cache.h"

int main()
{
  Patrick::Cache c;

  auto status = c.set("Patrick", "Madden");
  std::cout << "Set operation status: " << ((status == Patrick::Status::STORED) ? "STORED" : "ERROR") << std::endl;
  std::cout << "Patrick: " << *c.get("Patrick") << std::endl;

  auto status2 = c.add("Patrick", "xMadden");
  std::cout << "Set operation status: " << ((status2 == Patrick::Status::STORED) ? "STORED" : "ERROR") << std::endl;
  std::cout << "Patrick: " << *c.get("Patrick") << std::endl;
}
