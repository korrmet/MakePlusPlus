#include "builder.hpp"
#include "expression.hpp"
#include <cstdio>
#include <cstdlib>

#define root independency::path()

builder::builder(independency::storage& storage, independency::storage& temp)
: temp(temp)
{ this->storage = storage;

  for (std::string item : this->storage.ls(root/"build"))
  { this->storage[root/"build"/item/"marked"] = false; }

  std::list<std::string> objects;
  for (std::string object : this->storage.ls(root/"build"))
  { objects.push_back(object); }

  std::list<std::string> sources;
  for (std::string item : this->storage.ls(root/"build"))
  { for (std::string source : this->storage.ls(root/"build"/item/"sources"))
    { sources.push_back(this->storage[root/"build"/item/"sources"/source]); } }

  for (std::string object : objects)
  { temp[root/"files"/object/"exists"]
      = !(independency::file(object).operator!());
    temp[root/"files"/object/"crs"]
      = ckstring(independency::file(object).read()); }

  for (std::string source : sources)
  { temp[root/"files"/source/"exists"]
      = !(independency::file(source).operator!());
    temp[root/"files"/source/"crs"]
      = ckstring(independency::file(source).read()); } }

struct job
{ std::string name;
  std::list<std::string> commands; };

bool builder::build()
{ std::list<struct job> jobs; int count = 0;

  do
  { count = 0;

    for (std::string item : storage.ls(root/"build"))
    { if (storage[root/"build"/item/"marked"] == true) { continue; }
      
      bool deps_ok = true;
      for (std::string dep : storage.ls(root/"build"/item/"dependencies"))
      { if (storage[root/"build"/dep/"marked"] == false) { deps_ok = false; } }

      if (!deps_ok) { continue; }

      struct job current;
      current.name = item;
      for (std::string command : storage.ls(root/"build"/item/"commands"))
      { current.commands.push_back((std::string)storage
                                   [root/"build"/item/"commands"/command]); }
      storage[root/"build"/item/"marked"] = true;
      jobs.push_back(current); count++; }
  } while (count);

  for (std::string item : storage.ls(root/"build"))
  { if (storage[root/"build"/item/"marked"] == false)
    { std::printf("--> unresolved build_target \"%s\"\n", item.c_str());
      return false; } }

  for (struct job& current : jobs)
  { std::fflush(stdout);
    std::printf("Building %s ...\n", current.name.c_str());
    for (std::string command : current.commands)
    { std::string expanded;
      if (!expression(storage).expand(command, expanded)) { return false; }
      if (std::system(expanded.c_str()) != 0) { return false; }
    } }

  return true; }

void builder::clean()
{ for (std::string file : storage.ls(root/"build"))
  { std::printf("Deleting %s\n", file.c_str());
    independency::file(file).del(); } }

std::string builder::ckstring(std::string input)
{ char arr[21]; for (char& c : arr) { c = ' '; } arr[20] = 0;
  unsigned int count = 0;

  for (char c : input)
  { arr[count] += c; if (arr[count] < ' ') { arr[count] = ' '; }
    count++; count %= 20; }

  return std::string(arr); }
