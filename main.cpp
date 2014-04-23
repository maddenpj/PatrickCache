#include <iostream>
#include <sstream>
#include <deque>
#include <string>

#include "cache.h"
#include "periodic_executor.h"

using namespace Patrick;
typedef std::deque<String> ArgList;

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

template<class T>
struct ReplCommand
{
  String command;
  String help;
  int requiredArgs;
  int optionalArgs;
  std::function<String(std::shared_ptr<T>&, ArgList&, ArgList&)> func;

  ReplCommand(String command_, int requiredArgs_, int optionalArgs_, String help_, std::function<String(std::shared_ptr<T>&, ArgList&, ArgList&)> func_)
    : command(command_), requiredArgs(requiredArgs_), optionalArgs(optionalArgs_), help(help_), func(func_)
  {
  }

  String execute(std::shared_ptr<T>& c, String line)
  {
    std::istringstream iss(line);
    String command_in;
    std::deque<String> required;
    std::deque<String> optional;

    if(iss >> command_in) {
      if(command != command_in) {
        return "Unsupported";
      }

      for(int i = 0; i < requiredArgs; i++) {
        String arg;
        if(iss >> arg) {
          required.push_back(arg);
        } else {
          return "Insufficient Arguments";
        }
      }

      for(int i = 0; i < optionalArgs; i++) {
        String arg;
        if(iss >> arg) {
          optional.push_back(arg);
        } else {
          break;
        }
      }
    }

    return func(c, required, optional);
  }
};

std::vector<ReplCommand<Cache>> setupRepl();


int main()
{
  std::shared_ptr<Cache> c(new Cache);
  PeriodicExecutor<Cache> p(1, c, &Cache::cleanUp);
  std::cout << "PatrickCache started" << std::endl;

  auto commands = setupRepl();

  std::cout << "> ";
  String line;
  while(std::getline(std::cin, line)) {
    std::istringstream iss(line);
    String command;
    if(iss >> command) {
      if( command == "help") {
        for( auto& x : commands) {
          std::cout << x.help << std::endl;
        }
      }

      for( auto& x : commands) {
        if (x.command == command) {
          auto result = x.execute(c, line);
          std::cout << result << std::endl;
        }
      }
    }
    std::cout << "> ";
  }
}

std::vector<ReplCommand<Cache>> setupRepl() {
  std::vector<ReplCommand<Cache>> commands;

  commands.push_back(ReplCommand<Cache>("set",
    2, 1,
    "set\n\tset <key> <value> <expiration (optional in seconds)>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      auto value = required.front();
      required.pop_front();
      if(optional_.size() >= 1) {
        auto expiration = optional_.front();
        optional_.pop_front();
        auto op = optional<int>( std::stoi(expiration) );
        return status_to_string(c->set(key, value, op));
      } else {
        return status_to_string(c->set(key, value));
      }
    })
  );

  commands.push_back(ReplCommand<Cache>("get",
    1, 0,
    "get\n\tget <key>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      return option_to_string(c->get(key));
    })
  );

  commands.push_back(ReplCommand<Cache>("add",
    2, 1,
    "add\n\tadd <key> <value> <expiration (optional in seconds)>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      auto value = required.front();
      required.pop_front();
      if(optional_.size() >= 1) {
        auto expiration = optional_.front();
        optional_.pop_front();
        auto op = optional<int>( std::stoi(expiration) );
        return status_to_string(c->add(key, value, op));
      } else {
        return status_to_string(c->add(key, value));
      }
    })
  );

  commands.push_back(ReplCommand<Cache>("increment",
    1, 1,
    "increment\n\tincrement <key> <value (optional defaults to 1)>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      if(optional_.size() >= 1) {
        auto amount = optional_.front();
        optional_.pop_front();
        auto output = c->increment(key, amount);
        if(output.first == Status::STORED) {
          return std::to_string(output.second);
        } else {
          return status_to_string(output.first);
        }
      } else {
        auto output = c->increment(key);
        if(output.first == Status::STORED) {
          return std::to_string(output.second);
        } else {
          return status_to_string(output.first);
        }
      }
    })
  );

  commands.push_back(ReplCommand<Cache>("decrement",
    1, 1,
    "decrement\n\tdecrement <key> <value (optional defaults to 1)>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      if(optional_.size() >= 1) {
        auto amount = optional_.front();
        optional_.pop_front();
        auto output = c->decrement(key, amount);
        if(output.first == Status::STORED) {
          return std::to_string(output.second);
        } else {
          return status_to_string(output.first);
        }
      } else {
        auto output = c->decrement(key);
        if(output.first == Status::STORED) {
          return std::to_string(output.second);
        } else {
          return status_to_string(output.first);
        }
      }
    })
  );


  commands.push_back(ReplCommand<Cache>("delete",
    1, 0,
    "delete\n\tdelete <key>\n",
    [](std::shared_ptr<Cache>& c, ArgList& required, ArgList& optional_) -> String {
      auto key = required.front();
      required.pop_front();
      return status_to_string(c->deleteElem(key));
    })
  );

  return commands;
}
