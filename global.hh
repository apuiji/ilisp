#pragma once

#include<map>
#include"var.hh"

namespace zlt::ilisp::global {
  extern void *bp;
  extern void *sp;

  static inline Var *&vbp = *(Var **) &bp;
  static inline Var *&vsp = *(Var **) &sp;

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
