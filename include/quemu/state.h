// John Stanco 9.29.18

#include <complex>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef QUEMU_STATE_H_
#define QUEMU_STATE_H_

namespace quemu {

// Quantum state
typedef unsigned int qubit_t;
typedef std::complex<double> cx_double;

class State {
 public:
  explicit State(const qubit_t num_qubits);

  typedef std::vector<cx_double>::iterator iterator;
  typedef std::vector<cx_double>::const_iterator const_iterator;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  const cx_double &operator[](const size_t index) const;
  cx_double &operator[](const size_t index);

  qubit_t NumQubits() const;
  size_t Size() const;
  void Normalize();

  /// Copies state from other.  If the number of qubits don't match, this method
  /// will return false, and the state will not be modifed.
  bool CopyFrom(const State &other);

 private:
  std::vector<cx_double> states_;
  const qubit_t num_qubits_;
};

}  // namespace quemu

#endif  // QUEMU_STATE_H_
