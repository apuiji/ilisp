#pragma once

#include"var.hh"

namespace zlt::ilisp::vm {
  extern void *bp;
  extern void *sp;

  static inline Var *&vbp = *(Var **) &bp;
  static inline Var *&vsp = *(Var **) &sp;

  extern const char *pc;

  void push(const void *data, size_t size);

  template<class T>
  static inline void pushT(T t) {
    push(&t, sizeof(T));
  }

  static inline const void *read(size_t size) noexcept {
    auto p = pc;
    p += size;
    return p;
  }

  template<class T>
  static inline T readT() noexcept {
    return *(const T *) read(sizeof(T));
  }
}
