// John Stanco 9.29.18

#include "quemu/circuit.h"

#include <iostream>
#include <map>

#include "quemu/gate.h"
#include "quemu/state.h"

namespace quemu {

const double kPi = std::acos(-1);

bool Circuit::GateComparator::operator()(const GateSpecifier& lhs,
                                         const GateSpecifier& rhs) const {
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

bool Circuit::AddGate(std::unique_ptr<Gate> gate, const QubitList& qubits,
                      const uint32_t time) {
  // check for overlapping events ( from gates of different sizes )
  auto it = occupied_.find(time);
  if (it != occupied_.end()) {
    for (auto& qubit : qubits) {
      if (!it->second.insert(qubit).second) {
        return false;
      }
    }
  } else {
    occupied_.emplace(
        std::make_pair(time, std::set<qubit_t>{qubits.begin(), qubits.end()}));
  }
  return schedule_
      .emplace(std::make_pair(GateSpecifier{time, qubits}, std::move(gate)))
      .second;
}

bool Circuit::Transform(State& state) {
  for (auto& event : schedule_) {
    if (!event.second->Transform(state, event.first.qubits)) {
      return false;
    }
  }
  return true;
}

bool Circuit::Transform(const State& input, State& output) {
  return output.CopyFrom(input) && Transform(output);
}

CircuitBuilder::CircuitBuilder() : circuit_{nullptr}, good_{false} {}

CircuitBuilder& CircuitBuilder::BuildCircuit() {
  circuit_ = std::unique_ptr<Circuit>{new Circuit};
  good_ = true;
  return *this;
}

std::unique_ptr<Circuit> CircuitBuilder::Get() {
  if (good_) {
    return std::move(circuit_);
  }

  circuit_ = nullptr;
  return nullptr;
}

CircuitBuilder& CircuitBuilder::AddGate(std::unique_ptr<Gate> gate,
                                        const QubitList& qubits,
                                        const uint32_t time) {
  good_ = good_ && circuit_->AddGate(std::move(gate), qubits, time);
  return *this;
}

CircuitBuilder& CircuitBuilder::AddGateX(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi, 0, kPi);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateY(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi, kPi / 2, kPi / 2);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateZ(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateS(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi / 2);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateSd(const qubit_t qubit,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, -kPi / 2);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateT(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi / 4);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateTd(const qubit_t qubit,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, -kPi / 4);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateI(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, 0);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateH(const qubit_t qubit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi / 2, 0, kPi);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU1(const qubit_t qubit,
                                          const double phase,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, phase);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU2(const qubit_t qubit,
                                          const double phi1, const double phi2,
                                          uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi / 2, phi1, phi2);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU3(const qubit_t qubit,
                                          const double phi1, const double phi2,
                                          const double phi3,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(phi1, phi2, phi3);
  return AddGate(std::move(gate), {qubit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCX(const qubit_t qubit,
                                          const qubit_t control,
                                          const uint32_t time) {
  auto gate = std::make_unique<CUGate>(kPi, 0, kPi);
  return AddGate(std::move(gate), {qubit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU1(const qubit_t qubit,
                                           const qubit_t control,
                                           const double phase,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(0, 0, phase);
  return AddGate(std::move(gate), {qubit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU2(const qubit_t qubit,
                                           const qubit_t control,
                                           const double phi1, const double phi2,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(kPi / 2, phi1, phi2);
  return AddGate(std::move(gate), {qubit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU3(const qubit_t qubit,
                                           const qubit_t control,
                                           const double phi1, const double phi2,
                                           const double phi3,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(phi1, phi2, phi3);
  return AddGate(std::move(gate), {qubit, control}, time);
}
}  // namespace quemu
