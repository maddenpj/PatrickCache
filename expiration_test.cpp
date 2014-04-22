#include "cache.h"
#include "periodic_executor.h"


using namespace Patrick;

int main()
{
  std::shared_ptr<Cache> c(new Cache("Cut"));
  
  c->add("expires-4", "test", optional<int>(4));
  c->set("expires-2", "test2", optional<int>(2));
  std::cout << "get expires-4" << std::endl << *(c->get("expires-4")) << std::endl;

  PeriodicExecutor<Cache> p(1, c, &Cache::cleanUp);
  std::cout << "Cache started" << std::endl;
  
  std::cout << "get expires-4" << std::endl << *(c->get("expires-4")) << std::endl;
  while(true);
}
