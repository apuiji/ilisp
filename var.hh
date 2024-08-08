#pragma once

#include<string_view>
#include<variant>

namespace zlt::ilisp {
  struct Object;

  enum {
    NULL_INDEX,
    NUM_INDEX,
    STR_INDEX,
    OBJ_INDEX
  };

  using Var = std::variant<std::monostate, double, const std::string *, Object *>;

  // cast operations begin
  double toNum(const Var &var) noexcept;

  static inline int toInt(const Var &var) noexcept {
    return (int) toNum(var);
  }

  std::string_view toStr(const Var &var) noexcept;

  static inline bool toBool(const Var &var) noexcept {
    return var.index() != NULL_INDEX;
  }

  static inline Object *getObj(Var &var) noexcept {
    return *(Object **) &var;
  }

  static inline const Object *getObj(const Var &var) noexcept {
    return *(const Object **) &var;
  }

  static inline Object *getIfObj(Var &var) noexcept {
    return var.index() == OBJ_INDEX ? getObj(var) : nullptr;
  }

  static inline const Object *getIfObj(const Var &var) noexcept {
    return var.index() == OBJ_INDEX ? getObj(var) : nullptr;
  }

  template<class T>
  static inline T castObj(Var &var) noexcept {
    return static_cast<T>(getObj(var));
  }

  template<class T>
  static inline T castObj(const Var &var) noexcept {
    return static_cast<T>(getObj(var));
  }

  template<class T>
  static inline T castIfObj(Var &var) noexcept {
    return dynamic_cast<T>(getIfObj(var));
  }

  template<class T>
  static inline T castIfObj(const Var &var) noexcept {
    return dynamic_cast<T>(getIfObj(var));
  }
  // cast operations end

  // assign operations begin
  static inline void assignNull(Var &dest) noexcept {
    dest = std::monostate();
  }

  static inline void assignInt(Var &dest, int value) noexcept {
    dest = (double) value;
  }

  static inline void assignBool(Var &dest, bool value) noexcept {
    if (value) {
      dest = 1.;
    } else {
      assignNull(dest);
    }
  }
  // assign operations end

  // getter/setter begin
  Var getMemb(Var &var, const Var &memb) noexcept;
  void setMemb(Var &var, const Var &memb, const Var &value) noexcept;
  bool getLength(size_t &dest, const Var &var) noexcept;
  // getter/setter end

  bool compare(int &dest, const Var &a, const Var &b) noexcept;
}
