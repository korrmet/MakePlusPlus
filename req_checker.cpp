#include "req_checker.hpp"
#include <cstdio>
#include <fstream>
#include <regex>

#define root independency::path()

req_checker::req_checker(independency::storage& storage)
: storage(storage) {}

bool req_checker::check()
{ std::list<std::string> sources;
  
  for (std::string idx : storage.ls(root/"build"))
  { for (std::string source : storage.ls(root/"build"/idx/"sources"))
    { sources.push_back(storage[root/"build"/idx/"sources"/source]); } }

  std::list<std::string> requirements = storage.ls(root/"requirements");

  if (!requirements.size()) { return true; }

  bool result = true;

  for (std::string requirement : requirements)
  { unsigned int counter = 0;

    for (std::string source : sources)
    { std::ifstream file(source); if (!file) { continue; }
      std::regex re(std::string().append(">").append(requirement).append("<"));

      std::string line;
      while (std::getline(file, line))
      { std::smatch match;
        if (std::regex_search(line, match, re)) { counter++; } }

      file.close(); }

    if (!counter)
    { std::printf("--> requirement >%s< %s is not fulfilled\n",
                  requirement.c_str(),
                  ((std::string)
                   storage[root/"requirements"/requirement/"title"]).c_str());
      result = false; } }

  return result; }
