#pragma once

#include<map>
#include"var.hh"

namespace zlt::ilisp::global {
  extern Var *bp;
  extern Var *sp;
  extern const char *pc;
  extern std::map<const std::string *, Var> defs;

  void pushPtr(void *ptr);
  void pushPtr(const void *ptr);

  template<class T>
  static inline T readPC() noexcept {
    T t = *(const T *) pc;
    pc += sizeof(T);
    return t;
  }
}
