#pragma once

#include<map>
#include"var.hh"

namespace zlt::ilisp::global {
  extern std::map<const std::string *, Var> defs;
}
