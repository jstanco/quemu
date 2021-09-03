// John Stanco 9.29.18

#include <complex>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef QUEMU_GATE_H_
#define QUEMU_GATE_H_

namespace quemu {

typedef uint32_t qbit_t;
typedef std::complex<double> cx_double;
typedef std::vector<qbit_t> QbitList;
class State;

class Gate {
 public:
  virtual ~Gate() {}
  virtual bool Apply(State &state, const QbitList &qbits) = 0;
};

struct GateSpecifier {
  // specifies at what time ( discrete )
  const uint32_t time;
  // specifies which qbit to act on
  QbitList qbits;
  GateSpecifier(const uint32_t t, const QbitList &qs) : time{t}, qbits{qs} {}
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
    if (lhs.qbits.size() > rhs.qbits.size()) {
      return false;
    } else if (lhs.qbits.size() < rhs.qbits.size()) {
      return true;
    }

    // same length qbit list
    for (size_t i = 0; i < rhs.qbits.size(); ++i) {
      if (lhs.qbits[i] > rhs.qbits[i]) {
        return false;
      } else if (lhs.qbits[i] < rhs.qbits[i]) {
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
  bool Apply(State &state, const QbitList &qbits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

class CUGate : public Gate {
 public:
  CUGate(const double phi1, const double phi2, const double phi3);
  bool Apply(State &state, const QbitList &qbits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

}  // namespace quemu

#endif  // QUEMU_GATE_H_
