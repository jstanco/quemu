// John Stanco 9.29.18

#include "quemu/gate.h"

#include <iostream>

#include "quemu/state.h"

namespace quemu {

const cx_double I = cx_double{0, 1};
const double PI = std::acos(-1);

UGate::UGate(const double phi1, const double phi2, const double phi3) {
  const auto w1 = exp(I * phi1 * .5);
  const auto w2 = exp(I * phi2);
  const auto w3 = exp(I * phi3);
  u00_ = w1.real();
  u01_ = -w3 * w1.imag();
  u10_ = w2 * w1.imag();
  u11_ = w2 * w3 * w1.real();
}

bool UGate::Transform(State &state, const QbitList &qbits) {
  if (qbits.size() != 1 || qbits.front() >= state.NumQbits()) {
    return false;
  }

  const qbit_t NumQbits = state.NumQbits();
  const qbit_t qbit_index = qbits.front();

  const size_t stride = 1 << qbit_index;
  const size_t active_state = (1 << NumQbits) - stride;

  for (size_t i = 0; i < active_state; i += stride) {
    const auto max = i + stride;
    for (; i < max; ++i) {
      const auto j = i + stride;
      const auto a0 = state[i];
      const auto a1 = state[j];
      state[i] = u00_ * a0 + u01_ * a1;
      state[j] = u10_ * a0 + u11_ * a1;
    }
  }
  return true;
}

CUGate::CUGate(const double phi1, const double phi2, const double phi3) {
  const auto w1 = exp(I * phi1 * .5);
  const auto w2 = exp(I * phi2);
  const auto w3 = exp(I * phi3);
  u00_ = w1.real();
  u01_ = -w3 * w1.imag();
  u10_ = w2 * w1.imag();
  u11_ = w1 * w2 * w1.real();
}

bool CUGate::Transform(State &state, const QbitList &qbits) {
  if (qbits.size() != 2 || qbits.front() >= state.NumQbits() ||
      qbits.back() >= state.NumQbits()) {
    return false;
  }

  const auto NumQbits = state.NumQbits();
  const auto qbit_index = qbits.front();
  const auto control = qbits.back();

  const size_t stride = 1 << qbit_index;
  const size_t active_state = (1 << NumQbits) - stride;
  const size_t control_mask = (1 << control);

  for (size_t i = 0; i < active_state; i += stride) {
    const auto max = i + stride;
    for (; i < max; ++i) {
      if ((i & control_mask) >> control) {
        const auto j = i + stride;
        const auto a0 = state[i];
        const auto a1 = state[j];
        state[i] = u00_ * a0 + u01_ * a1;
        state[j] = u10_ * a0 + u11_ * a1;
      }
    }
  }
  return true;
}

}  // namespace quemu
