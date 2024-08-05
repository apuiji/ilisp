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
      vsp[-1] = toNum(vsp[-1]) + toNum(vsp[0]);
      --vsp;
    } else if (op == opcode::ARITH_SUB) {
      vsp[-1] = toNum(vsp[-1]) - toNum(vsp[0]);
      --vsp;
    } else if (op == opcode::ARITH_MUL) {
      vsp[-1] = toNum(vsp[-1]) * toNum(vsp[0]);
      --vsp;
    } else if (op == opcode::ARITH_DIV) {
      vsp[-1] = toNum(vsp[-1]) / toNum(vsp[0]);
      --vsp;
    } else if (op == opcode::ARITH_MOD) {
      vsp[-1] = fmod(toNum(vsp[-1]), toNum(vsp[0]));
      --vsp;
    } else if (op == opcode::ARITH_POW) {
      vsp[-1] = pow(toNum(vsp[-1]), toNum(vsp[0]));
      --vsp;
    }
    // arithmetical operations end
    // logical operations end
    else if (op == opcode::LOGIC_NOT) {
      assignBool(vsp[0], !toBool(vsp[0]));
    } else if (op == opcode::LOGIC_XOR) {
      assignBool(vsp[-1], toBool(vsp[-1]) ^ toBool(vsp[0]));
      --vsp;
    }
    // logical operations end
    // bitwise operations begin
    else if (op == opcode::BITWS_AND) {
      assignInt(vsp[-1], toInt(vsp[-1]) & toInt(vsp[0]));
      --vsp;
    } else if (op == opcode::BITWS_OR) {
      assignInt(vsp[-1], toInt(vsp[-1]) | toInt(vsp[0]));
      --vsp;
    } else if (op == opcode::BITWS_NOT) {
      assignInt(vsp[0], ~toInt(vsp[0]));
    } else if (op == opcode::BITWS_XOR) {
      assignInt(vsp[-1], toInt(vsp[-1]) ^ toInt(vsp[0]));
      --vsp;
    } else if (op == opcode::LSH) {
      assignInt(vsp[-1], toInt(vsp[-1]) << toInt(vsp[0]));
      --vsp;
    } else if (op == opcode::RSH) {
      assignInt(vsp[-1], toInt(vsp[-1]) >> toInt(vsp[0]));
      --vsp;
    } else if (op == opcode::USH) {
      assignInt(vsp[-1], (unsigned) toInt(vsp[-1]) >> toInt(vsp[0]));
      --vsp;
    }
    // bitwise operations end
    // comparisons begin
    else if (op == opcode::CMP_EQ) {
      int diff;
      assignBool(vsp[-1], compare(diff, vsp[-1], vsp[0]) && diff == 0);
      --vsp;
    } else if (op == opcode::CMP_LT) {
      int diff;
      assignBool(vsp[-1], compare(diff, vsp[-1], vsp[0]) && diff < 0);
      --vsp;
    } else if (op == opcode::CMP_GT) {
      int diff;
      assignBool(vsp[-1], compare(diff, vsp[-1], vsp[0]) && diff > 0);
      --vsp;
    } else if (op == opcode::CMP_LTEQ) {
      int diff;
      assignBool(vsp[-1], compare(diff, vsp[-1], vsp[0]) && diff <= 0);
      --vsp;
    } else if (op == opcode::CMP_GTEQ) {
      int diff;
      assignBool(vsp[-1], compare(diff, vsp[-1], vsp[0]) && diff >= 0);
      --vsp;
    } else if (op == opcode::CMP_3WAY) {
      int diff;
      if (compare(diff, vsp[-1], vsp[0])) {
        assignInt(vsp[-1], diff);
      } else {
        assignNull(vsp[-1]);
      }
      --vsp;
    }
    // comparisons end
    // getters begin
    else if (op == opcode::GET_LOCAL) {
      int i = readPC<int>();
      vsp[0] = vbp[i];
    } else if (op == opcode::GET_CLOSURE) {
      int i = readPC<int>();
      vsp[0] = castObj<FunctionObj *>(vbp[-1])->closures[i];
    } else if (op == opcode::GET_GLOBAL) {
      auto name = readPC<const string *>();
      vsp[0] = global::defs[name];
    } else if (op == opcode::GET_MEMB) {
      vsp[-1] = getMemb(vsp[-1], vsp[0]);
      --vsp;
    } else if (op == opcode::GET_REF) {
      vsp[0] = castObj<ReferenceObj *>(vsp[0])->value;
    }
    // getters end
    // setters begin
    else if (op == opcode::SET_LOCAL) {
      int i = readPC<int>();
      vbp[i] = vsp[0];
    } else if (op == opcode::SET_CLOSURE) {
      int i = readPC<int>();
      castObj<FunctionObj *>(vbp[-1])->closures[i] = vsp[0];
    } else if (op == opcode::SET_GLOBAL) {
      auto name = readPC<const string *>();
      global::defs[name] = vsp[0];
    } else if (op == opcode::SET_MEMB) {
      setMemb(vsp[-2], vsp[-1], vsp[0]);
      vsp[-2] = vsp[0];
      vsp -= 2;
    } else if (op == opcode::SET_REF) {
      castObj<ReferenceObj *>(vsp[-1])->value = vsp[0];
      --vsp;
    }
    // setters end
    else if (op == opcode::BEFORE_FORWARD) {
      size_t argc = readPC<size_t>();
      copy(vsp - argc - 1, vsp, vbp - 1);
      vsp = vbp + argc;
    } else if (op == opcode::BEFORE_RETURN) {
      vbp[0] = vsp[0];
      vsp = vbp;
    } else if (op == opcode::BEFORE_THROW) {
      // TODO
      ;
    } else if (op == opcode::CALL) {
      call();
      return;
    } else if (op == opcode::CLEAR_FN_GUARDS) {
      pushPtr(pc);
      // TODO
      pc = nullptr;
    } else if (op == opcode::INIT_DEFC) {
      size_t paramc = readPC<size_t>();
      size_t defc = readPC<size_t>();
      if (sp - bp > defc) {
        sp = bp + defc;
      }
      for (int i = paramc; i < defc; ++i) {
        assignNull(bp[i]);
      }
    } else if (op == opcode::JIF) {
      size_t n = readPC<size_t>();
      if (toBool(sp[0])) {
        pc += n;
      }
    } else if (op == opcode::JMP) {
      size_t n = readPC<size_t>();
      pc += n;
    } else if (op == opcode::LENGTH) {
      size_t n;
      if (getLength(n, sp[0])) {
        assignInt(sp[0], n);
      } else {
        sp[0] = NAN;
      }
    } else if (op == opcode::MAKE_FN) {
      size_t closurec = readPC<size_t>();
      size_t bodySize = readPC<size_t>();
      FunctionObj::Closures closures(new Var[closurec]);
      sp[0] = new FunctionObj(std::move(closures), pc);
      pc += bodySize;
    } else if (op == opcode::MAKE_REF) {
      int i = readPC<int>();
      auto ref = new ReferenceObj(bp[i]);
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
