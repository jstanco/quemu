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

  const cx_double &operator[](size_t index) const;
  cx_double &operator[](size_t index);

  qbit_t NumQbits() const;
  size_t Size() const;
  void Normalize();

 private:
  std::vector<cx_double> qbits_;
  qbit_t num_qbits_;

  void init();
};

}  // namespace quemu

#endif  // QUEMU_STATE_H_
