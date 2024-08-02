#pragma once

#include<memory>
#include"var.hh"

namespace zlt::ilisp {
  struct Object {
    virtual ~Object() = default;
  };

  struct FunctionObj final: Object {
    using Closures = std::unique_ptr<Var[]>;
    Closures closures;
    const char *body;
    FunctionObj(Closures &&closures, const char *body) noexcept: closures(std::move(closures)), body(body) {}
  };

  struct ReferenceObj final: Object {
    Var value;
    ReferenceObj() = default;
    ReferenceObj(const Var &value) noexcept: value(value) {}
  };
}
