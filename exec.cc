#include<cmath>
#include"commons/opcode.hh"
#include"global.hh"
#include"ilisp.hh"
#include"object.hh"

using namespace std;

namespace zlt::ilisp {
  using namespace global;

  static void call();

  void exec() {
    int op = *pc;
    ++pc;
    // arithmetical operations begin
    if (op == opcode::ARITH_ADD) {
      sp[-1] = toNum(sp[-1]) + toNum(sp[0]);
      --sp;
    } else if (op == opcode::ARITH_SUB) {
      sp[-1] = toNum(sp[-1]) - toNum(sp[0]);
      --sp;
    } else if (op == opcode::ARITH_MUL) {
      sp[-1] = toNum(sp[-1]) * toNum(sp[0]);
      --sp;
    } else if (op == opcode::ARITH_DIV) {
      sp[-1] = toNum(sp[-1]) / toNum(sp[0]);
      --sp;
    } else if (op == opcode::ARITH_MOD) {
      sp[-1] = fmod(toNum(sp[-1]), toNum(sp[0]));
      --sp;
    } else if (op == opcode::ARITH_POW) {
      sp[-1] = pow(toNum(sp[-1]), toNum(sp[0]));
      --sp;
    }
    // arithmetical operations end
    // logical operations end
    else if (op == opcode::LOGIC_NOT) {
      sp[0] = !toBool(sp[0]);
    } else if (op == opcode::LOGIC_XOR) {
      setBool(sp[-1], toBool(sp[-1]) ^ toBool(sp[0]));
      --sp;
    }
    // logical operations end
    // bitwise operations begin
    else if (op == opcode::BITWS_AND) {
      sp[-1] = toInt(sp[-1]) & toInt(sp[0]);
      --sp;
    } else if (op == opcode::BITWS_OR) {
      sp[-1] = toInt(sp[-1]) | toInt(sp[0]);
      --sp;
    } else if (op == opcode::BITWS_NOT) {
      sp[0] = ~toInt(sp[0]);
    } else if (op == opcode::BITWS_XOR) {
      sp[-1] = toInt(sp[-1]) ^ toInt(sp[0]);
      --sp;
    } else if (op == opcode::LSH) {
      sp[-1] = toInt(sp[-1]) << toInt(sp[0]);
      --sp;
    } else if (op == opcode::RSH) {
      sp[-1] = toInt(sp[-1]) >> toInt(sp[0]);
      --sp;
    } else if (op == opcode::USH) {
      sp[-1] = (unsigned) toInt(sp[-1]) >> toInt(sp[0]);
      --sp;
    }
    // bitwise operations end
    // comparisons begin
    else if (op == opcode::CMP_EQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff == 0);
      --sp;
    } else if (op == opcode::CMP_LT) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff < 0);
      --sp;
    } else if (op == opcode::CMP_GT) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff > 0);
      --sp;
    } else if (op == opcode::CMP_LTEQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff <= 0);
      --sp;
    } else if (op == opcode::CMP_GTEQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff >= 0);
      --sp;
    } else if (op == opcode::CMP_3WAY) {
      int diff;
      if (compare(diff, sp[-1], sp[0])) {
        setInt(sp[-1], diff);
      } else {
        setNull(sp[-1]);
      }
      --sp;
    }
    // comparisons end
    // getters begin
    else if (op == opcode::GET_LOCAL) {
      int i = readPC();
      sp[0] = bp[i];
    } else if (op == opcode::GET_CLOSURE) {
      int i = readPC();
      sp[0] = castObj<FunctionObj *>(bp[-1])->closures[i];
    } else if (op == opcode::GET_GLOBAL) {
      const string *name = readPC();
      sp[0] = global::defs[name];
    } else if (op == opcode::GET_MEMB) {
      sp[-1] = getMemb(sp[-1], sp[0]);
      --sp;
    } else if (op == opcode::GET_REF) {
      sp[0] = *toPtr<Var *>(sp[0]);
    }
    // getters end
    // setters begin
    else if (op == opcode::SET_LOCAL) {
      int i = readPC();
      bp[i] = sp[0];
    } else if (op == opcode::SET_CLOSURE) {
      int i = readPC();
      castObj<FunctionObj *>(bp[-1])->closures[i] = sp[0];
    } else if (op == opcode::SET_GLOBAL) {
      const string *name = readPC();
      global::defs[name] = sp[0];
    } else if (op == opcode::SET_MEMB) {
      setMemb(sp[-2], sp[-1], sp[0]);
      sp[-2] = sp[0];
      sp -= 2;
    } else if (op == opcode::SET_REF) {
      *toPtr<Var *>(sp[-1]) = sp[0];
      --sp;
    }
    // setters end
    else if (op == opcode::CALL) {
      call();
      return;
    } else if (op == opcode::CLEAR_FN_GUARDS) {
      pushPtr(pc);
      // TODO
      pc = nullptr;
    } else if (op = opcode::INIT_DEFC) {
      size_t paramc = readPC();
      size_t defc = readPC();
      if (sp - bp > defc) {
        sp = bp + defc;
      }
      for (int i = paramc; i < defc; ++i) {
        setNull(bp[i]);
      }
    } else if (op == opcode::INIT_FORWARD) {
      size_t argc = readPC();
      copy(sp - argc - 1, sp, bp - 1);
      sp = bp + argc;
    } else if (op == opcode::JIF) {
      size_t n = readPC();
      if (toBool(sp[0])) {
        pc += n;
      }
    } else if (op == opcode::JMP) {
      size_t n = readPC();
      pc += n;
    } else if (op == opcode::LENGTH) {
      size_t n;
      if (getLength(n, sp[0])) {
        setInt(sp[0], n);
      } else {
        sp[0] = NAN;
      }
    } else if (op == opcode::MAKE_FN) {
      size_t closurec = readPC();
      size_t bodySize = readPC();
      FunctionObj::Closures closures(new Var[closurec]);
      sp[0] = new FunctionObj(std::move(closures), pc);
      pc += bodySize;
    } else if (op == opcode::MAKE_REF) {
      int i = readPC();
      auto ref = new Reference(bp[i]);
      bp[i] = ref;
    } else if (op == opcode::NEGATIVE) {
      sp[0] = -toNum(sp[0]);
    } else if (op == opcode::POP) {
      --sp;
    } else if (op == opcode::POSITIVE) {
      sp[0] = toNum(sp[0]);
    } else if (op == opcode::PUSH) {
      // TODO
      ++sp;
    } else if (op == opcode::PUSH_CATCH) {
      // TODO
    } else if (op == opcode::PUSH_DEFER) {
      // TODO
    } else if (op == opcode::PUSH_GUARD) {
      // TODO
    } else if (op == opcode::RETURN) {
      // TODO
    } else if (op == opcode::THROW) {
      // TODO
    }
    exec();
  }
}
