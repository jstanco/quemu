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
  Circuit() = default;
  Circuit(const Circuit &other) = delete;
  Circuit &operator=(Circuit other) = delete;

  bool Apply(State &state);

 private:
  friend class CircuitBuilder;
  bool AddGate(std::unique_ptr<Gate> gate, const QbitList &qbits,
               const uint32_t time);

  GateSchedule schedule_;
  std::map<uint32_t, std::set<qbit_t> > occupied_;
};

class CircuitBuilder {
 public:
  CircuitBuilder();

  // Resets builder state
  CircuitBuilder &BuildCircuit();

  // Generic add function
  CircuitBuilder &AddGate(std::unique_ptr<Gate>, const QbitList &qbits,
                          const uint32_t time);

  std::unique_ptr<Circuit> Get();

  // Single qbit gates
  CircuitBuilder &AddGateX(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateY(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateZ(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateS(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateSd(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateT(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateTd(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateID(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateH(const qbit_t qbit, const uint32_t time);
  CircuitBuilder &AddGateU1(const qbit_t qbit, const double phase,
                            const uint32_t time);
  CircuitBuilder &AddGateU2(const qbit_t qbit, const double phi1,
                            const double phi2, const uint32_t time);
  CircuitBuilder &AddGateU3(const qbit_t qbit, const double phi1,
                            const double phi2, const double phi3,
                            const uint32_t time);

  // Double qbit gates
  CircuitBuilder &AddGateCX(const qbit_t qbit, const qbit_t control,
                            const uint32_t time);
  CircuitBuilder &AddGateCU1(const qbit_t qbit, const qbit_t control,
                             const double phase, uint32_t time);
  CircuitBuilder &AddGateCU2(const qbit_t qbit, const qbit_t control,
                             const double phi1, const double phi2,
                             const uint32_t time);
  CircuitBuilder &AddGateCU3(const qbit_t qbit, const qbit_t control,
                             const double phi1, const double phi2,
                             const double phi3, const uint32_t time);

 private:
  std::unique_ptr<Circuit> circuit_;
  bool good_;
};

}  // namespace quemu

#endif  // QUEMU_CIRCUIT_H_
