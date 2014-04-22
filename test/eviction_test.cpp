#include "cache.h"

#include <string>

using namespace Patrick;

int main()
{
  Cache c;
  
  for(int i = 0; i < 2000000; i++) {
    c.set(std::to_string(i), "12345678910");
  }
  
  std::cout << "Cache " << c.getCacheSize() << std::endl;
  c.garbageCollect(); 
  std::cout << "Cache " << c.getCacheSize() << std::endl;
}
