#include<cmath>
#include"commons/opcode.hh"
#include"global.hh"
#include"ilisp.hh"
#include"var.hh"

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
      exec();
    } else if (op == opcode::ARITH_SUB) {
      sp[-1] = toNum(sp[-1]) - toNum(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::ARITH_MUL) {
      sp[-1] = toNum(sp[-1]) * toNum(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::ARITH_DIV) {
      sp[-1] = toNum(sp[-1]) / toNum(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::ARITH_MOD) {
      sp[-1] = fmod(toNum(sp[-1]), toNum(sp[0]));
      --sp;
      exec();
    } else if (op == opcode::ARITH_POW) {
      sp[-1] = pow(toNum(sp[-1]), toNum(sp[0]));
      --sp;
      exec();
    }
    // arithmetical operations end
    // bitwise operations begin
    else if (op == opcode::BITWS_AND) {
      sp[-1] = toInt(sp[-1]) & toInt(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::BITWS_OR) {
      sp[-1] = toInt(sp[-1]) | toInt(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::BITWS_NOT) {
      sp[0] = ~toInt(sp[0]);
      exec();
    } else if (op == opcode::BITWS_XOR) {
      sp[-1] = toInt(sp[-1]) ^ toInt(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::LSH) {
      sp[-1] = toInt(sp[-1]) << toInt(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::RSH) {
      sp[-1] = toInt(sp[-1]) >> toInt(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::USH) {
      sp[-1] = (unsigned) toInt(sp[-1]) >> toInt(sp[0]);
      --sp;
      exec();
    }
    // bitwise operations end
    // comparisons begin
    else if (op == opcode::CMP_EQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff == 0);
      --sp;
      exec();
    } else if (op == opcode::CMP_LT) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff < 0);
      --sp;
      exec();
    } else if (op == opcode::CMP_GT) {
      ;
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff > 0);
      --sp;
      exec();
    } else if (op == opcode::CMP_LTEQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff <= 0);
      --sp;
      exec();
    } else if (op == opcode::CMP_GTEQ) {
      int diff;
      setBool(sp[-1], compare(diff, sp[-1], sp[0]) && diff >= 0);
      --sp;
      exec();
    } else if (op == opcode::CMP_3WAY) {
      int diff;
      if (compare(diff, sp[-1], sp[0])) {
        setInt(sp[-1], diff);
      } else {
        setNull(sp[-1]);
      }
      --sp;
      exec();
    }
    // comparisons end
    // getters begin
    else if (op == opcode::GET_LOCAL) {
      int i = readPC();
      sp[0] = bp[i];
      exec();
    } else if (op == opcode::GET_CLOSURE) {
      int i = readPC();
      sp[0] = toObj<FunctionObj>(bp[-1])->closures[i];
      exec();
    } else if (op == opcode::GET_GLOBAL) {
      const string *name = readPC();
      sp[0] = global::defs[name];
      exec();
    } else if (op == opcode::GET_MEMB) {
      sp[-1] = getMemb(sp[-1], sp[0]);
      --sp;
      exec();
    } else if (op == opcode::GET_REF) {
      sp[0] = *toPtr<Var *>(sp[0]);
      exec();
    }
    // getters end
    // setters begin
    else if (op == opcode::SET_LOCAL) {
      int i = readPC();
      bp[i] = sp[0];
      exec();
    } else if (op == opcode::SET_CLOSURE) {
      int i = readPC();
      toObj<FunctionObj>(bp[-1])->closures[i] = sp[0];
      exec();
    } else if (op == opcode::SET_GLOBAL) {
      const string *name = readPC();
      global::defs[name] = sp[0];
      exec();
    } else if (op == opcode::SET_MEMB) {
      setMemb(sp[-2], sp[-1], sp[0]);
      sp[-2] = sp[0];
      sp -= 2;
      exec();
    } else if (op == opcode::SET_REF) {
      *toPtr<Var *>(sp[-1]) = sp[0];
      --sp;
      exec();
    }
    // setters end
    else if (op == opcode::CALL) {
      call();
      return;
    } else if (op == opcode::CLEAR_FN_GUARDS) {
      pushPtr(pc);
      // TODO
      pc = nullptr;
      exec();
    } else if (op = opcode::)

  }
}
