// John Stanco 9.29.18

#include <complex>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef QUEMU_STATE_H_
#define QUEMU_STATE_H_

namespace quemu {

using qubit_t = uint32_t;

/// Represents tensor product state of a quantum register with N qubits. For a
/// register with N qubits, the dimension of the joint state is 2 ^ N, where
/// each index represents the complex amplitude corresponding to a particular
/// combination of all N qubits.
class State final {
 private:
 public:
  /// Initializes the state with fixed number of qubits.  At initialization, the
  /// tensor product state will have a complex probability with a value of one
  /// in the zeroth index. (Would be measured in the zeroth state with
  /// probability one).
  explicit State(const qubit_t num_qubits);

  /// Allows iteration over the underlying vector memory
  typedef std::vector<std::complex<double>>::iterator iterator;

  /// Allows iteration over the underlying vector memory
  typedef std::vector<std::complex<double>>::const_iterator const_iterator;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  /// Accesses complex amplitude at index in tensor product state.  This
  /// operator is not bounds checked, and the index must fall in the range
  /// [0, 2 ^ N), where N is the number of qubits in the state.
  const std::complex<double> &operator[](const size_t index) const;

  /// Accesses complex amplitude at index in tensor product state.  This
  /// operator is not bounds checked, and the index must fall in the range
  /// [0, 2 ^ N), where N is the number of qubits in the state.
  std::complex<double> &operator[](const size_t index);

  /// Returns number of qubits in register
  qubit_t NumQubits() const;

  /// Returns dimension of tensor product state.  This will be 2 ^ N, where N is
  /// the number of qubits in the register.
  size_t Size() const;

  /// Normalize the state coefficients such that the norm of the state equals
  /// one.
  void Normalize();

  /// Copies state from other.  If the number of qubits don't match, this method
  /// will return false, and the state will not be modifed.
  bool CopyFrom(const State &other);

 private:
  std::vector<std::complex<double>> states_;
  const qubit_t num_qubits_;
};

}  // namespace quemu

#endif  // QUEMU_STATE_H_
