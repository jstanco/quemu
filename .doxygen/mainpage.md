# quemu
**Quemu** is an object-oriented quantum circuit simulation framework, providing utility for building and running custom quantum circuits. In quemu, a user is able to incrementally create quantum gates and add them to a custom circuit.  To do so, the user constructs an instance of \link quemu::CircuitBuilder `CircuitBuilder`\endlink, and adds instances of \link quemu::Gate `Gate`\endlink.  After the appropriate gates have been added, the user calls \link quemu::CircuitBuilder::Get `CircuitBuilder::Get` \endlink to retrieve a finished \link quemu::Circuit `Circuit` \endlink instance.

```cc
// Use CircuitBuilder to construct a Circuit instance.
auto builder = CircuitBuilder();
auto circuit = builder.BuildCircuit()
                     .AddGateX(1, 0)
                     .AddGateY(2, 0)
                     .Get();
```

An instance of \link quemu::Circuit `Circuit` \endlink can be used to transform an instance of \link quemu::State `State`\endlink, through the \link quemu::Circuit::Transform `Circuit::Transform` \endlink method.  When invoked on a single instance of \link quemu::State `State`\endlink, the transform is performed in place, in which case the state belongs to the circuit's input register prior to the transform, and the output register after the transform. 

```cc
const auto num_qubits = 4;

// In-place transform
auto state = State(num_qubits);
const auto success = circuit->Transform(state);
```

When performing the transform out-of-place, the input and output registers are defined separately.

```cc
const auto num_qubits = 4;

// Out-of-place transform
auto input = State(num_qubits);
auto output = State(num_qubits);
const auto success = circuit->Transform(input, output);
```
