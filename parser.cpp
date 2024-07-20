#include "Independency/independency.hpp"
#include <cstdio>
#include <fstream>

class mpp_parser
{ public:
  mpp_parser(independency::storage& storage)
  : storage(storage), current_mode(mode::idle) {}
  
  void input(std::string line)
  { if (line == "# CONFIGURATION")
    { current_mode = mode::configuration;
      std::printf("--> switch to configuration mode\n");
      return; }

    if (line == "# REQUIREMENTS")
    { current_mode = mode::requirements;
      std::printf("--> switch to requirements mode\n");
      return; }

    if (line == "# BUILD")
    { current_mode = mode::build;
      std::printf("--> switch to build mode\n");
      return; }

    if (line == "# TEST")
    { current_mode = mode::test;
      std::printf("--> switch to test mode\n");
      return; }

    if (line == "# DEPLOY")
    { current_mode = mode::deploy;
      std::printf("--> switch to deploy mode\n");
      return; }

    switch (current_mode)
    { case mode::idle: break;
      case mode::configuration: break;
      case mode::requirements: break;
      case mode::build: break;
      case mode::test: break;
      case mode::deploy: break;
      default: break; }
  }

  private:
  enum class mode { configuration, requirements, build, test, deploy, idle }
  current_mode;
  independency::storage& storage;

  std::list<std::string> tokenizer(std::string line)
  { std::list<str::string> tokens;

    return tokens; }
};

int main(int argc, char** argv)
{ independency::cli_parser parser(argc, argv);

  if (!parser.count("--in")) { std::printf("Nothing to process\n"); return 0; }

  std::string filename = parser["--in"];
  std::ifstream file(filename);
  if (!file) { std::printf("Can't open %s\n", filename.c_str()); }

  independency::storage stor;
  mpp_parser mp(stor);
  std::string line;
  while (std::getline(file, line))
  { std::printf("%s\n", line.c_str()); mp.input(line); }

  return 0; }
