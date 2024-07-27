#include "expression.hpp"

#define root independency::path()

expression::expression(independency::storage& storage) : storage(storage) {}

bool expression::expand(std::string expression, std::string& result)
{ bool expanded = true;

  while (expanded)
  { expanded = false; bool escape = false; result.clear();

    for (unsigned int i = 0; i < expression.size(); i++)
    { if (escape) { result.push_back(expression[i]); escape = false; }
      
      if (expression[i] == '\\') { escape = true; continue; }
      else if (expression[i] == '$')
      { expanded = true; i++;
        std::string variable; bool terminated = false;
        for (unsigned int j = i; j < expression.size(); j++, i++)
        { if (expression[j] == '$') { terminated = true; break; }
          variable.push_back(expression[j]); }

        if (!terminated)
        { std::printf("--> unterminated expression\n"); return false; }

        if (!storage.chk(root/"variables"/variable))
        { std::printf("--> unknown variable \"%s\"\n", variable.c_str());
          return false; }

        result.append((std::string)storage[root/"variables"/variable]); }

      else { result.push_back(expression[i]); } }

    expression = result; }

  return true; }
