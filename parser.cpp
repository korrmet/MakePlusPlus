#include "Independency/independency.hpp"
#include <cstdio>
#include <fstream>

class mpp_parser
{ public:
  mpp_parser(independency::storage& storage)
  : storage(storage), current_mode(mode::idle) {}
  
  bool input(std::string line)
  { if (line[0] == '#')
    { std::printf("--> comment\n"); return true; }

    if (line[0] == '*')
    { if (line == "* CONFIGURATION")
      { current_mode = mode::configuration;
        std::printf("--> switch to configuration mode\n");
        return true; }

      else if (line == "* REQUIREMENTS")
      { current_mode = mode::requirements;
        std::printf("--> switch to requirements mode\n");
        return true; }

      else if (line == "* BUILD")
      { current_mode = mode::build;
        std::printf("--> switch to build mode\n");
        return true; }

      else if (line == "* TEST")
      { current_mode = mode::test;
        std::printf("--> switch to test mode\n");
        return true; }

      else if (line == "* DEPLOY")
      { current_mode = mode::deploy;
        std::printf("--> switch to deploy mode\n");
        return false; }

      else
      { std::printf("--> Unknown section header \"%s\"\n", line.c_str());
        return false; }
    }

    switch (current_mode)
    { case mode::idle: break;

      case mode::configuration:
      { std::printf("--> Tokens:\n");
        unsigned int count = 0;
        std::list<std::string> tokens = conf_tokenizer(line);
        for (std::string token : tokens)
        { std::printf("--> %3d: %s\n", count++, token.c_str()); }

        conf_translator(tokens);
      } break;
      
      case mode::requirements: break;
      case mode::build: break;
      case mode::test: break;
      case mode::deploy: break;
      default: break; }

    return true; }

  private:
  enum class mode { configuration, requirements, build, test, deploy, idle }
  current_mode;
  independency::storage& storage;

  enum class conf_command
  { unknown, variable, assign, menu_item, link_variable, add_description,
    add_default, add_case };

  std::list<std::string> conf_tokenizer(std::string line)
  { std::list<std::string> tokens;

    std::string current; bool inside_braces = false; bool escape = false;

    for (char c : line)
    { if (escape) { current.push_back(c); escape = false; continue; }
      if (inside_braces)
      { if (c == '\\') { escape = true; continue; }
        if (c == '"') { inside_braces = false; continue; } }
      else
      { if (c == ' ' || c == '\t')
        { if (current.size()) { tokens.push_back(current); }
          current.clear();
          continue; } }

      if (c == '"') { inside_braces = true; continue; }

      current.push_back(c); }

    if (current.size()) { tokens.push_back(current); }

    return tokens; }

  bool conf_translator(std::list<std::string> tokens)
  { unsigned int pos = 0;

    conf_command current = conf_command::unknown;
    std::string context;

    for (std::string token : tokens)
    { if (pos == 0) // command
      { std::printf("--> \"%s\" command given\n", token.c_str());
        
        if (token == "variable")
        { if (tokens.size() < 2)
          { std::printf("-->\"variable\" command requires an argument(s)\n");
            return false; }
         
          current = conf_command::variable;
          context.clear(); }

        else if (token == "assign")
        { if (tokens.size() < 2)
          { std::printf("--> \"assign\" command requires at least one "
                        "argument\n");
            return false; }

          if (tokens.size() > 3)
          { std::printf("--> \"assign\" command supports no more than two "
                        "arguments\n");
            return false; }
        
          current = conf_command::assign; }

        else if (token == "menu_item")
        { if (tokens.size() < 2)
          { std::printf("--> \"menu_item\" command requires at least one "
                        "argument\n");
            return false; }

          current = conf_command::menu_item;
          context.clear(); }

        else if (token == "link_variable")
        { if (tokens.size() < 2)
          { std::printf("--> \"link_variable\" command requires at least one "
                        "argument\n");
            return false; }

          if (tokens.size() > 3)
          { std::printf("--> \"link_variable\" command supports no more than "
                        "two arguments\n");
            return false; }

          current = conf_command::link_variable;
          context.clear(); }

        else if (token == "add_description")
        { if (tokens.size() < 2)
          { std::printf("--> \"add_description\" command requires at least one "
                        "argument\n");
            return false; }

          if (tokens.size() > 3)
          { std::printf("--> \"add_description\" command supports no more than "
                        "two arguments\n");
            return false; }

          current = conf_command::add_description; }

        else if (token == "add_default")
        { if (tokens.size() < 2)
          { std::printf("--> \"add_default\" command requires at least one "
                        "argument\n");
            return false; }

          if (tokens.size() > 3)
          { std::printf("--> \"add_default\" command supports no more than two "
                        "arguments\n");
            return false; }

          current = conf_command::add_default; }

        else if (token == "add_case")
        { if (tokens.size() < 2)
          { std::printf("--> \"add_case\" command requires at least one "
                        "argument\n");
            return false; }

          if (tokens.size() > 3)
          { std::printf("--> \"add_case\" command supports no more than two "
                        "arguments\n");
            return false; }
        
          current = conf_command::add_case; }

        else { std::printf("--> unknown command: \"%s\"\n", token.c_str());
               return false; } }

      else // arguments
      { if (current == conf_command::variable)
        { std::printf("--> creating variable \"%s\"\n", token.c_str());
          context = token; }

        if (current == conf_command::assign)
        { if (pos == 1) { std::printf("--> assignment value \"%s\"\n",
                                      token.c_str()); }
          if (pos == 2) { std::printf("--> assignment target \"%s\"\n",
                                      token.c_str()); }

          if (tokens.size() == 2 && pos == 1)
          { std::printf("--> execute assignment to context\n"); }

          if (tokens.size() == 3 && pos == 2)
          { std::printf("--> execute assignment to specified variable\n"); } }

        if (current == conf_command::menu_item)
        { std::printf("--> creating menu item \"%s\"\n", token.c_str());
          context = token; }

        if (current == conf_command::link_variable)
        { if (pos == 1) { std::printf("--> linking value \"%s\"\n",
                                      token.c_str()); }
          if (pos == 2) { std::printf("--> linking target \"%s\"\n",
                                      token.c_str()); }

          if (token.size() == 2 && pos == 1)
          { std::printf("--> execute variable linking to context\n"); }

          if (token.size() == 3 && pos == 2)
          { std::printf("--> execute variable linking to menu item\n"); } }
      }

      pos++; }

    return true; }
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
  { std::printf("%s\n", line.c_str());
    if (!mp.input(line)) { break; } }

  return 0; }
