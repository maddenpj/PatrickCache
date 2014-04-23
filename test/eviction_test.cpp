#include "cache.h"

#include <string>

using namespace Patrick;

int main()
{
  Cache c;

  c.set("Patrick", "Madden");
  c.get("Patrick");
  c.get("Patrick");
  c.get("Patrick");
  c.get("Patrick");
  
  for(int i = 0; i < 1500000; i++) {
    c.set(std::to_string(i), "12345678910");
  }
  
  std::cout << "Cache Size: " << c.getCacheSize() << std::endl;
  c.garbageCollect(); 
  std::cout << "Cache Size After GC: " << c.getCacheSize() << std::endl;
  std::cout << "Accessing more used element: " << *c.get("Patrick") << std::endl;
}
