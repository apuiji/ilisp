#pragma once

#include<map>
#include"var.hh"

namespace zlt::ilisp::global {
  extern char *bp;
  extern char *sp;
  extern const char *pc;
  extern std::map<const std::string *, Var> defs;

  void push(const void *data, size_t size);

  template<class T>
  static inline void push(T t) {
    push(&t, sizeof(T));
  }

  template<class T>
  static inline T readPC() noexcept {
    T t = *(const T *) pc;
    pc += sizeof(T);
    return t;
  }
}
