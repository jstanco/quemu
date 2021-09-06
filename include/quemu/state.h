// John Stanco 9.29.18

#include <complex>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef QUEMU_STATE_H_
#define QUEMU_STATE_H_

namespace quemu {

// Quantum state
typedef unsigned int qbit_t;
typedef std::complex<double> cx_double;

class State {
 public:
  explicit State(const qbit_t num_qbits);

  typedef std::vector<cx_double>::iterator iterator;
  typedef std::vector<cx_double>::const_iterator const_iterator;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  const cx_double &operator[](const size_t index) const;
  cx_double &operator[](const size_t index);

  qbit_t NumQbits() const;
  size_t Size() const;
  void Normalize();

  /// Copies state from other.  If the number of qbits don't match, this method
  /// will return false, and the state will not be modifed.
  bool CopyFrom(const State &other);

 private:
  std::vector<cx_double> states_;
  const qbit_t num_qbits_;
};

}  // namespace quemu

#endif  // QUEMU_STATE_H_
