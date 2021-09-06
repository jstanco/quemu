// John Stanco 9.29.18

#include <complex>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef QUEMU_GATE_H_
#define QUEMU_GATE_H_

namespace quemu {

typedef uint32_t qubit_t;
typedef std::complex<double> cx_double;
typedef std::vector<qubit_t> QubitList;
class State;

class Gate {
 public:
  virtual ~Gate() {}
  virtual bool Transform(State &state, const QubitList &qubits) = 0;
};

struct GateSpecifier {
  // specifies at what time ( discrete )
  const uint32_t time;
  // specifies which qubit to act on
  QubitList qubits;
  GateSpecifier(const uint32_t t, const QubitList &qs) : time{t}, qubits{qs} {}
};

struct GateComparator {
  // less than
  bool operator()(const GateSpecifier &lhs, const GateSpecifier &rhs) const {
    if (lhs.time > rhs.time) {
      return false;
    } else if (lhs.time < rhs.time) {
      return true;
    }

    // same time
    if (lhs.qubits.size() > rhs.qubits.size()) {
      return false;
    } else if (lhs.qubits.size() < rhs.qubits.size()) {
      return true;
    }

    // same length qubit list
    for (size_t i = 0; i < rhs.qubits.size(); ++i) {
      if (lhs.qubits[i] > rhs.qubits[i]) {
        return false;
      } else if (lhs.qubits[i] < rhs.qubits[i]) {
        return true;
      }
    }
    // equal
    return false;
  }
};

class UGate : public Gate {
 public:
  UGate(const double phi1, const double phi2, const double phi3);
  bool Transform(State &state, const QubitList &qubits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

class CUGate : public Gate {
 public:
  CUGate(const double phi1, const double phi2, const double phi3);
  bool Transform(State &state, const QubitList &qubits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

}  // namespace quemu

#endif  // QUEMU_GATE_H_
