#include<cmath>
#include"commons/opcode.hh"
#include"global.hh"
#include"ilisp.hh"
#include"value.hh"

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
    } else if (op == opcode::ARITH_DIV) {
      sp[-1] = toNum(sp[-1]) / toNum(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::ARITH_MOD) {
      sp[-1] = fmod(toNum(sp[-1]), toNum(sp[0]));
      --sp;
      exec();
    } else if (op == opcode::ARITH_MUL) {
      sp[-1] = toNum(sp[-1]) * toNum(sp[0]);
      --sp;
      exec();
    } else if (op == opcode::ARITH_POW) {
      sp[-1] = pow(toNum(sp[-1]), toNum(sp[0]));
      --sp;
      exec();
    } else if (op == opcode::ARITH_SUB) {
      sp[-1] = toNum(sp[-1]) - toNum(sp[0]);
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
    else if (op == opcode::CMP_3WAY) {
      int diff;
      if ()
    }
    ,
    CMP_EQ,
    CMP_GT,
    CMP_GTEQ,
    CMP_LT,
    CMP_LTEQ,
    // comparisons end
    else if (op == opcode::CALL) {
      call();
      return;
    } else if (op == opcode::CLEAR_FN_GUARDS) {
      pushPtr(pc);
      // TODO
      pc = nullptr;
      exec();
    } else if ()

  }
}
