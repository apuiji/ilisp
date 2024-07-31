#pragma once

#include"var.hh"

namespace zlt::ilisp::global {
  extern Var *bp;
  extern Var *sp;
  extern const char *pc;

  template<class T>
  static inline T readPC() noexcept {
    T t = *(const T *) pc;
    pc += sizeof(T);
    return pc;
  }
}
