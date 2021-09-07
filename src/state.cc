// John Stanco 9.29.18

#include "quemu/state.h"

#include <cstring>

namespace quemu {
namespace {
using cx_double = std::complex<double>;
}

State::State(const qubit_t num_qubits)
    : states_{std::vector<cx_double>(1 << num_qubits)},
      num_qubits_{num_qubits} {
  states_[0] = 1;
}

State::iterator State::begin() { return states_.begin(); }
State::const_iterator State::begin() const { return states_.begin(); }
State::iterator State::end() { return states_.end(); }
State::const_iterator State::end() const { return states_.end(); }

const cx_double& State::operator[](const size_t index) const {
  return states_[index];
}

cx_double& State::operator[](const size_t index) { return states_[index]; }

qubit_t State::NumQubits() const { return num_qubits_; }
size_t State::Size() const { return states_.size(); }

bool State::CopyFrom(const State& other) {
  if (NumQubits() != other.NumQubits()) {
    return false;
  }

  const auto num_bytes = states_.size() * sizeof(cx_double);
  return std::memcpy(states_.data(), other.states_.data(), num_bytes) !=
         nullptr;
}

void State::Normalize() {
  double norm = 0;
  for (auto& z : states_) {
    norm += std::norm(z);
  }
  if (norm != 1) {
    for (auto& coeff : states_) {
      coeff /= norm;
    }
  }
}

}  // namespace quemu
