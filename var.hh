#pragma once

#include<string_view>
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

  using Var = std::variant<std::monostate, double, const std::string *, Object *, void *>;

  // cast operations begin
  double toNum(const Var &var) noexcept;
  int toInt(int &dest, const Var &var) noexcept;
  std::string_view toStr(std::string_view &dest, const Var &var) noexcept;
  bool toBool(const Var &var) noexcept;
  // cast operations end

  // set operations begin
  static inline void setNull(Var &dest) noexcept {
    dest = std::monostate();
  }

  static inline void setInt(Var &dest, int value) noexcept {
    dest = (double) value;
  }

  static inline void setBool(Var &dest, bool value) noexcept {
    if (value) {
      dest = 1;
    } else {
      setNull(dest);
    }
  }
  // set operations end

  bool compare(int &dest, const Var &a, const Var &b) noexcept;
}
