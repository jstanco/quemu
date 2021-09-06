# quemu

**Quemu** is an object-oriented quantum circuit simulation framework, providing utility for building and running custom quantum circuits. In quemu, a user is able to incrementally create quantum gates and add them to a custom circuit.  To do so, the user constructs an instance of `CircuitBuilder`, and adds instances of `Gate`.  After the appropriate gates have been added, the user calls `CircuitBuilder::Get`to retrieve a finished `Circuit` instance.

```cc
// Use CircuitBuilder to construct a Circuit instance.
auto builder = CircuitBuilder();
auto circuit = builder.BuildCircuit()
                     .AddGateX(1, 0)
                     .AddGateY(2, 0)
                     .Get();
```

An instance of `Circuit` can be used to transform an instance of `State`, through the `Circuit::Transform` method.  When invoked on a single instance of `State`, the transform is performed in place, in which case the state belongs to the circuit's input register prior to the transform, and the output register after the transform. 

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

## Documentation

Further documentation can be found [here](https://jstanco.github.io/quemu/index.html)
