#include<cmath>
#include<cstring>
#include"commons/opcode.hh"
#include"global.hh"
#include"ilisp.hh"
#include"object.hh"
#include"vm.hh"

using namespace std;

namespace zlt::ilisp {
  using namespace vm;

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
      int i = readT<int>();
      vsp[0] = vbp[i];
    } else if (op == opcode::GET_CLOSURE) {
      int i = readT<int>();
      vsp[0] = castObj<FunctionObj *>(vbp[-1])->closures[i];
    } else if (op == opcode::GET_GLOBAL) {
      auto name = readT<const string *>();
      vsp[0] = global::defs[name];
    } else if (op == opcode::GET_MEMB) {
      vsp[-1] = getMemb(vsp[-1], vsp[0]);
      --vsp;
    } else if (op == opcode::GET_REF) {
      vsp[0] = castObj<ReferenceObj *>(vsp[0])->value;
    }
    // getters end
    // setters begin
    else if (op == opcode::SET_CLOSURE) {
      int i = readT<int>();
      castObj<FunctionObj *>(vbp[-1])->closures[i] = vsp[0];
    } else if (op == opcode::SET_GLOBAL) {
      auto name = readT<const string *>();
      global::defs[name] = vsp[0];
    } else if (op == opcode::SET_LOCAL) {
      int i = readT<int>();
      vbp[i] = vsp[0];
    } else if (op == opcode::SET_MEMB) {
      setMemb(vsp[-2], vsp[-1], vsp[0]);
      vsp[-2] = vsp[0];
      vsp -= 2;
    } else if (op == opcode::SET_NULL) {
      vsp[0] = monostate();
    } else if (op == opcode::SET_NUM) {
      vsp[0] = readT<double>();
    } else if (op == opcode::SET_REF) {
      castObj<ReferenceObj *>(vsp[-1])->value = vsp[0];
      --vsp;
    } else if (op == opcode::SET_STR) {
      vsp[0] = readT<const string *>();
    }
    // setters end
    else if (op == opcode::BEFORE_FORWARD) {
      size_t argc = readT<size_t>();
      memcpy(vbp - 1, vsp - argc - 1, sizeof(Var) * (argc + 1));
      vsp = vbp + argc;
    } else if (op == opcode::CALL) {
      size_t argc = read<size_t>();
      vbp = vsp - argc;
      pc = castObj<FunctionObj *>(vbp[-1])->body;
    } else if (op == opcode::INIT_DEFC) {
      size_t paramc = readT<size_t>();
      size_t defc = readT<size_t>();
      if (vsp - vbp > defc) {
        vsp = vbp + defc;
      }
      for (int i = paramc; i < defc; ++i) {
        assignNull(vbp[i]);
      }
    } else if (op == opcode::JIF) {
      size_t n = readT<size_t>();
      if (toBool(vsp[0])) {
        pc += n;
      }
    } else if (op == opcode::JMP) {
      size_t n = readT<size_t>();
      pc += n;
    } else if (op == opcode::LENGTH) {
      size_t n;
      if (getLength(n, vsp[0])) {
        assignInt(vsp[0], n);
      } else {
        vsp[0] = NAN;
      }
    } else if (op == opcode::MAKE_FN) {
      size_t closurec = readT<size_t>();
      size_t bodySize = readT<size_t>();
      FunctionObj::Closures closures(new Var[closurec]);
      vsp[0] = new FunctionObj(std::move(closures), pc);
      pc += bodySize;
    } else if (op == opcode::MAKE_REF) {
      int i = readT<int>();
      auto ref = new ReferenceObj(vbp[i]);
      vbp[i] = ref;
    } else if (op == opcode::NEGATIVE) {
      vsp[0] = -toNum(vsp[0]);
    } else if (op == opcode::POP) {
      --vsp;
    } else if (op == opcode::POSITIVE) {
      vsp[0] = toNum(vsp[0]);
    } else if (op == opcode::PUSH) {
      pushT(vsp[0]);
    } else if (op == opcode::PUSH_BP) {
      pushT(bp);
    } else if (op == opcode::PUSH_JMP) {
      size_t n = readT<size_t>();
      pushT(pc + n);
    } else if (op == opcode::RETURN) {
      struct T {
        void *bp;
        const char *pc;
        Var callee;
      };
      bp -= sizeof(T);
      T &t = *(T *) bp;
      bp = t.bp;
      pc = t.pc;
      vbp[0] = vsp[0];
      sp = bp;
    }
    exec();
  }
}
