#pragma once

#include<variant>

namespace zlt::ilisp {
  struct Object;

  enum {
    NULL_INDEX,
    NUM_INDEX,
    STR_INDEX,
    OBJ_INDEX,
    REF_INDEX
  };

  using Value = std::variant<std::monostate, double, const std::string *, Object *, void *>;

  void exec();
}
