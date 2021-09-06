// John Stanco 9.29.18

#include <cstdint>
#include <map>
#include <memory>
#include <set>

#include "quemu/gate.h"
#include "quemu/state.h"

#ifndef QUEMU_CIRCUIT_H_
#define QUEMU_CIRCUIT_H_

namespace quemu {

typedef std::complex<double> cx_double;
typedef std::map<GateSpecifier, std::unique_ptr<Gate>, GateComparator>
    GateSchedule;

class Circuit {
 public:
  // Circuits are non-copyable
  Circuit(const Circuit &other) = delete;
  Circuit &operator=(Circuit other) = delete;

  /// In-place application of circuit.  Before the transform, 'state' belongs
  /// to the input register.  After the transform, 'state' belongs to the
  /// output register.
  bool Transform(State &state);

  /// Out-of-place application of circuit. 'input' belongs to the input
  /// register, and 'output' belongs to the output register.
  bool Transform(const State &input, State &output);

 private:
  friend class CircuitBuilder;

  Circuit() = default;
  bool AddGate(std::unique_ptr<Gate> gate, const QbitList &qbits,
               const uint32_t time);

  GateSchedule schedule_;
  std::map<uint32_t, std::set<qbit_t> > occupied_;
};

class CircuitBuilder {
 public:
  CircuitBuilder();

  /// Resets internal builder state.  All previously added will be cleared.
  CircuitBuilder &BuildCircuit();

  /// Add Gate instance to the circuit acting on a set of qbits at a specific
  /// time.
  CircuitBuilder &AddGate(std::unique_ptr<Gate>, const QbitList &qbits,
                          const uint32_t time);

  /// Return newly created Circuit object.  If the builder state is invalid due
  /// to an improperly added gate, this call will return nullptr.
  std::unique_ptr<Circuit> Get();

  // Convenience methods for creating single qbit gates.

  /// Pauli X-gate
  CircuitBuilder &AddGateX(const qbit_t qbit, const uint32_t time);

  /// Pauli Y-gate
  CircuitBuilder &AddGateY(const qbit_t qbit, const uint32_t time);

  /// Pauli Z-gate
  CircuitBuilder &AddGateZ(const qbit_t qbit, const uint32_t time);

  /// S-Gate (square root of Z)
  CircuitBuilder &AddGateS(const qbit_t qbit, const uint32_t time);

  /// Sd-Gate (inverse of S-Gate)
  CircuitBuilder &AddGateSd(const qbit_t qbit, const uint32_t time);

  /// T-Gate (4th root of Z)
  CircuitBuilder &AddGateT(const qbit_t qbit, const uint32_t time);

  // Td-Gate (inverse of T-Gate)
  CircuitBuilder &AddGateTd(const qbit_t qbit, const uint32_t time);

  /// Identity gate
  CircuitBuilder &AddGateI(const qbit_t qbit, const uint32_t time);

  /// Add single-qbit Hadamard gate
  CircuitBuilder &AddGateH(const qbit_t qbit, const uint32_t time);

  /// Unitary gate with one phase parameter
  CircuitBuilder &AddGateU1(const qbit_t qbit, const double phase,
                            const uint32_t time);

  /// Unitary gate with two phase parameters
  CircuitBuilder &AddGateU2(const qbit_t qbit, const double phi1,
                            const double phi2, const uint32_t time);

  /// Unitary gate with three phase parameters
  CircuitBuilder &AddGateU3(const qbit_t qbit, const double phi1,
                            const double phi2, const double phi3,
                            const uint32_t time);

  /// Controlled Pauli X-gate
  CircuitBuilder &AddGateCX(const qbit_t qbit, const qbit_t control,
                            const uint32_t time);

  /// Controlled unitary gate with one phase parameter (P-gate)
  CircuitBuilder &AddGateCU1(const qbit_t qbit, const qbit_t control,
                             const double phase, uint32_t time);

  /// Controlled unitary gate with two phase parameters
  CircuitBuilder &AddGateCU2(const qbit_t qbit, const qbit_t control,
                             const double phi1, const double phi2,
                             const uint32_t time);

  /// Controlled unitary gate with three phase parameters
  CircuitBuilder &AddGateCU3(const qbit_t qbit, const qbit_t control,
                             const double phi1, const double phi2,
                             const double phi3, const uint32_t time);

 private:
  std::unique_ptr<Circuit> circuit_;
  bool good_;
};

}  // namespace quemu

#endif  // QUEMU_CIRCUIT_H_
