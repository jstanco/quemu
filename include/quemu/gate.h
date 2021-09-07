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

/// Abstract base class representing a quantum gate.
class Gate {
 public:
  virtual ~Gate() {}

  // Transform input state in-place acting on desired qubits.
  virtual bool Transform(State &state, const QubitList &qubits) = 0;
};

/// Unitary 3-parameter quantum gate
class UGate : public Gate {
 public:
  UGate(const double phi1, const double phi2, const double phi3);
  bool Transform(State &state, const QubitList &qubits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

/// Controlled unitary 3-parameter quantum gate
class CUGate : public Gate {
 public:
  CUGate(const double phi1, const double phi2, const double phi3);
  bool Transform(State &state, const QubitList &qubits) override;

 private:
  cx_double u00_, u01_, u10_, u11_;
};

}  // namespace quemu

#endif  // QUEMU_GATE_H_
