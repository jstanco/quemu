// John Stanco 9.29.18

#include "quemu/circuit.h"

#include <iostream>
#include <map>

#include "quemu/gate.h"
#include "quemu/state.h"

namespace quemu {

const double kPi = std::acos(-1);

bool Circuit::AddGate(std::unique_ptr<Gate> gate, const QbitList& qbits,
                      const uint32_t time) {
  // check for overlapping events ( from gates of different sizes )
  auto it = occupied_.find(time);
  if (it != occupied_.end()) {
    for (auto& qbit : qbits) {
      if (!it->second.insert(qbit).second) {
        return false;
      }
    }
  } else {
    occupied_.emplace(
        std::make_pair(time, std::set<qbit_t>{qbits.begin(), qbits.end()}));
  }
  return schedule_
      .emplace(std::make_pair(GateSpecifier{time, qbits}, std::move(gate)))
      .second;
}

/*
bool Circuit::Remove(const QbitList& qbits, const uint32_t time) {
  auto it = schedule_.find(GateSpecifier{time, qbits});
  if (it == schedule_.end()) {
    return false;
  }

  schedule_.erase(it);
  return true;
}

bool Circuit::Replace(std::unique_ptr<Gate> gate, const QbitList& qbits,
                      const uint32_t time) {
  auto it = schedule_.find(GateSpecifier{time, qbits});
  if (it == schedule_.end()) {
    // gate not found
    return false;
  }

  // gate found, replace
  it->second = std::move(gate);
  return true;
}
*/

bool Circuit::Transform(State& state) {
  for (auto& event : schedule_) {
    if (!event.second->Transform(state, event.first.qbits)) {
      return false;
    }
  }
  return true;
}

CircuitBuilder::CircuitBuilder() : circuit_{nullptr}, good_{false} {}

CircuitBuilder& CircuitBuilder::BuildCircuit() {
  circuit_ = std::make_unique<Circuit>();
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
                                        const QbitList& qbits,
                                        const uint32_t time) {
  good_ = good_ && circuit_->AddGate(std::move(gate), qbits, time);
  return *this;
}

CircuitBuilder& CircuitBuilder::AddGateX(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi, 0, kPi);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateY(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi, kPi / 2, kPi / 2);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateZ(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateS(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi / 2);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateSd(const qbit_t qbit,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, -kPi / 2);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateT(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, kPi / 4);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateTd(const qbit_t qbit,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, -kPi / 4);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateI(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, 0);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateH(const qbit_t qbit,
                                         const uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi / 2, 0, kPi);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU1(const qbit_t qbit, const double phase,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(0, 0, phase);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU2(const qbit_t qbit, const double phi1,
                                          const double phi2, uint32_t time) {
  auto gate = std::make_unique<UGate>(kPi / 2, phi1, phi2);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateU3(const qbit_t qbit, const double phi1,
                                          const double phi2, const double phi3,
                                          const uint32_t time) {
  auto gate = std::make_unique<UGate>(phi1, phi2, phi3);
  return AddGate(std::move(gate), {qbit}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCX(const qbit_t qbit,
                                          const qbit_t control,
                                          const uint32_t time) {
  auto gate = std::make_unique<CUGate>(kPi, 0, kPi);
  return AddGate(std::move(gate), {qbit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU1(const qbit_t qbit,
                                           const qbit_t control,
                                           const double phase,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(0, 0, phase);
  return AddGate(std::move(gate), {qbit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU2(const qbit_t qbit,
                                           const qbit_t control,
                                           const double phi1, const double phi2,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(kPi / 2, phi1, phi2);
  return AddGate(std::move(gate), {qbit, control}, time);
}

CircuitBuilder& CircuitBuilder::AddGateCU3(const qbit_t qbit,
                                           const qbit_t control,
                                           const double phi1, const double phi2,
                                           const double phi3,
                                           const uint32_t time) {
  auto gate = std::make_unique<CUGate>(phi1, phi2, phi3);
  return AddGate(std::move(gate), {qbit, control}, time);
}
}  // namespace quemu
