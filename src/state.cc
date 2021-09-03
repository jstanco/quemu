// John Stanco 9.29.18

#include "quemu/state.h"

namespace quemu {

State::State(const qbit_t num_qbits)
    : qbits_{std::vector<cx_double>(1 << num_qbits)}, num_qbits_{num_qbits} {
  qbits_[0] = 1;
}

State::iterator State::begin() { return qbits_.begin(); }
State::const_iterator State::begin() const { return qbits_.begin(); }
State::iterator State::end() { return qbits_.end(); }
State::const_iterator State::end() const { return qbits_.end(); }

const cx_double& State::operator[](size_t index) const { return qbits_[index]; }
cx_double& State::operator[](size_t index) { return qbits_[index]; }

qbit_t State::NumQbits() const { return num_qbits_; }
size_t State::Size() const { return qbits_.size(); }

void State::Normalize() {
  double norm = 0;
  for (auto& z : qbits_) {
    norm += std::norm(z);
  }
  if (norm != 1) {
    for (auto& coeff : qbits_) {
      coeff /= norm;
    }
  }
}

}  // namespace quemu
