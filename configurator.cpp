#include "configurator.hpp"
#include <cstdio>
#include <iostream>

configurator::configurator(independency::storage& storage,
                           independency::storage& temp)
: storage(storage), temp(temp), force_flag(false) { }

bool configurator::run()
{ if (!storage.chk(root/"menu items")) { return false; }

  for (std::string item : storage.ls(root/"menu items"))
  { bool skip_input = !force_flag && temp.chk(root/"menu items"/item);

    std::string value;
    if (!skip_input)
    { if (storage.chk(root/"menu items"/item/"description"))
      { std::printf("%s:\n",
                    ((std::string)
                     storage[root/"menu items"/item/"description"]).c_str());

        if (storage.ls(root/"menu items"/item/"cases").size())
        { std::printf("Supported values (you may enter your value instead):\n");
          for (std::string num : storage.ls(root/"menu items"/item/"cases"))
          { std::printf("%s\n",
                        ((std::string)storage[root/"menu items"/item/"cases"/num])
                        .c_str()); } } }
      else { std::printf("%s:\n", item.c_str()); }

      std::fflush(stdout); std::getline(std::cin, value);

      temp[root/"menu items"/item] = value; }
    else { value = (std::string)temp[root/"menu items"/item]; }

    if (storage.chk(root/"menu items"/item/"variable"))
    { std::string variable = storage[root/"menu items"/item/"variable"];
      if (!storage.chk(root/"variables"/variable))
      { std::printf("--> unknown variable \"%s\"\n", variable.c_str());
        return false; }

      storage[root/"variables"/variable] = value; }
    else { std::printf("--> no variable to configure\n"); return false; }
  }

  return true; }

configurator& configurator::force(bool enable)
{ force_flag = enable; return *this; }
