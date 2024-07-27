#ifndef PARSER_HPP
#define PARSER_HPP

#include "Independency/independency.hpp"
#include <cstdio>
#include <vector>

#define root independency::path()

class mpp_parser
{ public:
  mpp_parser(independency::storage& storage);

  bool input(std::string line);

  private:
  enum class mode { configuration, requirements, build, test, deploy, idle }
  current_mode;

  independency::storage& storage;
  std::string context;
  enum class conf_command
  { unknown, variable, assign, menu_item, link_variable, add_descriprion,
    add_default, add_case };

  std::vector<std::string> tokenizer(std::string line);
  bool conf_translator(std::vector<std::string> tokens);

  enum class req_parsing_stage
  { empty_line, index, title, description } req_stage;
  std::string req_idx;
  bool req_translator(std::string line);

  std::string build_target;
  bool build_translator(std::vector<std::string> tokens);

  std::string test_target;
  bool test_translator(std::vector<std::string> tokens);

  std::string deploy_target;
  bool deploy_translator(std::vector<std::string> tokens);
};

#endif // PARSER_HPP
