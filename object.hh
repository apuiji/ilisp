#pragma once

#include<memory>
#include"var.hh"

namespace zlt::ilisp {
  struct Object {
    virtual ~Object() = default;
    virtual double toNum() const noexcept;
    virtual std::string_view toStr() const noexcept;
    virtual Var getMemb(const Var &memb) const noexcept;
    virtual void setMemb(const Var &memb, const Var &value);
    virtual bool getLength(size_t &dest) const noexcept;
    virtual void gc()
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
