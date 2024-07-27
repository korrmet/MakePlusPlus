#include "parser.hpp"
#include <cstdio>

mpp_parser::mpp_parser(independency::storage& storage)
: storage(storage), current_mode(mode::idle),
  req_stage(req_parsing_stage::empty_line) {}
  
bool mpp_parser::input(std::string line)
{ if (line[0] == '#') { return true; }

  if (line[0] == '*')
  { if (line == "* CONFIGURATION")
    { current_mode = mode::configuration;
      return true; }

    else if (line == "* REQUIREMENTS")
    { current_mode = mode::requirements;
      return true; }

    else if (line == "* BUILD")
    { current_mode = mode::build;
      return true; }

    else if (line == "* TEST")
    { current_mode = mode::test;
      return true; }

    else if (line == "* DEPLOY")
    { current_mode = mode::deploy;
      return false; }

    else
    { std::printf("--> Unknown section header \"%s\"\n", line.c_str());
      return false; } }

  switch (current_mode)
  { case mode::idle: break;

    case mode::configuration: return conf_translator(tokenizer(line));
    case mode::requirements: return req_translator(line);
    case mode::build: return build_translator(tokenizer(line));
    case mode::test: return test_translator(tokenizer(line));
    case mode::deploy: return deploy_translator(tokenizer(line));
    default: break; }

  return true; }

std::vector<std::string> mpp_parser::tokenizer(std::string line)
{ std::vector<std::string> tokens;

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

bool mpp_parser::conf_translator(std::vector<std::string> tokens)
{ if (!tokens.size()) { return true; }

  if (tokens[0] == "variable")
  { if (tokens.size() <= 1)
    { std::printf("--> 'variable' command requires at least one argument\n");
      return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { storage[root/"variables"/tokens[i]] = ""; context = tokens[i]; } }

  else if (tokens[0] == "assign")
  { if (tokens.size() == 2)
    { if (context.empty())
      { std::printf("--> context is empty, nowhere to assign\n");
        return false; }

      if (!storage.chk(root/"variables"/context))
      { std::printf("--> unknown variable \"%s\"\n", context.c_str());
        return false; }

      storage[root/"variables"/context] = tokens[1]; }
    
    else if (tokens.size() == 3)
    { if (!storage.chk(root/"variables"/tokens[2]))
      { std::printf("--> unknown variable \"%s\"\n", tokens[2].c_str());
        return false; }

      storage[root/"variables"/tokens[2]] = tokens[1]; }

    else { std::printf("--> 'assign' command requires 2 or 3 argments\n");
           return false; } }

  else if (tokens[0] == "menu_item")
  { if (tokens.size() <= 1)
    { std::printf("--> 'menu_item' command requires at least one "
                  "argument\n");
      return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { storage[root/"menu items"/tokens[i]] = ""; context = tokens[i]; } }
  
  else if (tokens[0] == "link_variable")
  { if (tokens.size() == 2)
    { if (context.empty()) 
      { std::printf("--> context is empty, nowhere to link\n");
        return false; }

      if (!storage.chk(root/"menu items"/context))
      { std::printf("--> unknown menu item \"%s\"\n", context.c_str());
        return false; }

      storage[root/"menu items"/context/"variable"] = tokens[1]; }

    else if (tokens.size() == 3)
    { if (!storage.chk(root/"menu items"/tokens[2]))
      { std::printf("--> unknown menu item \"%s\"\n", tokens[2].c_str());
        return false; }

      storage[root/"menu items"/tokens[2]] = tokens[1]; }

    else { std::printf("--> 'link_variable' command requires 2 or 3 "
                       "arguments\n");
           return false; } }
  
  else if (tokens[0] == "add_description")
  { if (tokens.size() == 2)
    { if (context.empty())
      { std::printf("--> context is empty, nowhere to add description\n");
        return false; }

      if (!storage.chk(root/"menu items"/context))
      { std::printf("--> unknown menu item \"%s\"\n", context.c_str());
        return false; }

      storage[root/"menu items"/context/"description"] = tokens[1]; }
    
    else if (tokens.size() == 3)
    { if (!storage.chk(root/"menu items"/tokens[2]))
      { std::printf("--> unknown menu item \"%s\"\n", tokens[2].c_str());
        return false; }

      storage[root/"menu items"/tokens[2]/"description"] = tokens[1]; }

    else { std::printf("--> 'add_description' command requires 2 or 3 "
                       "arguments\n"); } }
  
  else if (tokens[0] == "add_default")
  { if (tokens.size() == 2)
    { if (context.empty())
      { std::printf("--> context is empty, nowhere to add default\n");
        return false; }

      if (!storage.chk(root/"menu items"/context))
      { std::printf("--> unknown menu item \"%s\"\n", context.c_str());
        return false; }

      storage[root/"menu items"/context/"default"] = tokens[1]; }

    else if (tokens.size() == 3)
    { if (!storage.chk(root/"menu items"/tokens[2]))
      { std::printf("--> unknown menu item \"%s\"\n", tokens[2].c_str());
        return false; }

      storage[root/"menu items"/tokens[2]/"default"] = tokens[1]; }

    else { std::printf("--> 'add_default' command requires 2 or 3 "
                       "arguments\n"); } }
  
  else if (tokens[0] == "add_case")
  { if (tokens.size() == 2)
    { if (context.empty())
      { std::printf("--> context is empty, nowhere to add case\n");
        return false; }

      if (!storage.chk(root/"menu items"/context))
      { std::printf("--> unknown menu item \"%s\"\n", context.c_str());
        return false; }

      int index = storage.ls(root/"menu items"/context/"cases").size();
      storage[root/"menu items"/context/"cases"/index] = tokens[1]; }

    else if (tokens.size() == 3)
    { if (!storage.chk(root/"menu items"/tokens[2]))
      { std::printf("--> unknown menu item \"%s\"\n", tokens[2].c_str());
        return false; }

      int index = storage.ls(root/"menu items"/tokens[2]/"cases").size();
      storage[root/"menu items"/tokens[2]/"cases"/index] = tokens[1]; }

    else { std::printf("--> 'add_case' command requires 2 or 3 "
                       "arguments\n"); } }
  
  else { std::printf("--> unknown command \"%s\"\n", tokens[0].c_str());
         return false; }

  return true; }

bool mpp_parser::req_translator(std::string line)
{ if (line.empty()) { req_stage = req_parsing_stage::empty_line; return true; }

  if (req_stage == req_parsing_stage::empty_line)
  { req_stage = req_parsing_stage::index; }
  else if (req_stage == req_parsing_stage::index)
  { req_stage = req_parsing_stage::title; }
  else if (req_stage == req_parsing_stage::title)
  { req_stage = req_parsing_stage::description; }

  if (req_stage == req_parsing_stage::index) { req_idx = line; }
  else if (req_stage == req_parsing_stage::title)
  { storage[root/"requirements"/req_idx/"title"] = line; }
  else if (req_stage == req_parsing_stage::description)
  { std::string description
    = storage[root/"requirements"/req_idx/"description"];
    description.append(line);
    storage[root/"requirements"/req_idx/"description"] = description; }

  return true; }

bool mpp_parser::build_translator(std::vector<std::string> tokens)
{ if (!tokens.size()) { return true; }

  if (tokens[0] == "target")
  { if (tokens.size() != 2)
    { std::printf("--> 'target' command requires one argument only\n");
      return false; }

    build_target = tokens[1]; }
  
  else if (tokens[0] == "command")
  { if (tokens.size() != 2)
    { std::printf("--> 'command' command requires one argument only\n");
      return false; }

    if (build_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    int idx = storage.ls(root/"build"/build_target/"commands").size();
    storage[root/"build"/build_target/"commands"/idx] = tokens[1]; }
  
  else if (tokens[0] == "source")
  { if (tokens.size() < 2)
    { std::printf("--> 'source' command requires at least one argument\n");
      return false; }

    if (build_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { int idx = storage.ls(root/"build"/build_target/"sources").size();
      storage[root/"build"/build_target/"sources"/idx] = tokens[i]; } }
  
  else if (tokens[0] == "dependency")
  { if (tokens.size() < 2)
    { std::printf("--> 'dependency' command requires at least one argument\n");
      return false; }

    if (build_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { storage[root/"build"/build_target/"dependencies"/tokens[i]]; } }

  else
  { std::printf("--> unknown command \"%s\"\n", tokens[0].c_str());
    return false; }

  return true; }

bool mpp_parser::test_translator(std::vector<std::string> tokens)
{ if (!tokens.size()) { return true; }

  if (tokens[0] == "target")
  { if (tokens.size() != 2)
    { std::printf("--> 'target' command requires one argument only\n");
      return false; }

    test_target = tokens[1]; }

  else if (tokens[0] == "command")
  { if (tokens.size() != 2)
    { std::printf("--> 'command' command requires one argument only\n");
      return false; }

    if (test_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    int idx = storage.ls(root/"tests"/test_target/"commands").size();
    storage[root/"tests"/test_target/"commands"/idx] = tokens[1]; }

  else if (tokens[0] == "source")
  { if (tokens.size() < 2)
    { std::printf("--> 'source' command requires at least one argument\n");
      return false; }

    if (test_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { int idx = storage.ls(root/"tests"/test_target/"sources").size();
      storage[root/"tests"/test_target/"sources"/idx] = tokens[i]; } }

  else if (tokens[0] == "dependency")
  { if (tokens.size() < 2)
    { std::printf("--> 'dependency' command requires at least one argument\n");
      return false; }

    if (test_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { int idx = storage.ls(root/"tests"/test_target/"dependencies").size();
      storage[root/"tests"/test_target/"dependencies"/idx] = tokens[i]; } }

  else
  { std::printf("--> unknown command \"%s\"\n", tokens[0].c_str());
    return false; }

  return true; }

bool mpp_parser::deploy_translator(std::vector<std::string> tokens)
{ if (!tokens.size()) { return true; }

  if (tokens[0] == "target")
  { if (tokens.size() != 2)
    { std::printf("--> 'target' command requires one argument only\n");
      return false; }

    deploy_target = tokens[1]; }

  else if (tokens[0] == "command")
  { if (tokens.size() != 2)
    { std::printf("--> 'command' command requires one argument only\n");
      return false; }

    if (deploy_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    int idx = storage.ls(root/"deployment"/deploy_target/"commands").size();
    storage[root/"deployment"/deploy_target/"commands"/idx] = tokens[1]; }

  else if (tokens[0] == "build")
  { if (tokens.size() < 2)
    { std::printf("--> 'build' command requires at least one argument\n");
      return false; }

    if (deploy_target.empty())
    { std::printf("--> target is not specified\n"); return false; }

    for (unsigned int i = 1; i < tokens.size(); i++)
    { int idx = storage.ls(root/"deployment"/deploy_target/"objects").size();
      storage[root/"deployment"/deploy_target/"objects"/idx] = tokens[i]; } }

  return true; }
