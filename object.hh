#pragma once

#include<memory>
#include"var.hh"

namespace zlt::ilisp {
  struct FunctionObj {
    using Closures = std::unique_ptr<Var[]>;
    Closures closures;
    const char *body;
    FunctionObj(Closures &&closures, const char *body) noexcept: closures(std::move(closures)), body(body) {}
  };
}
