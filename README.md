# quemu
Quemu is an object-oriented quantum circuit simulation framework, providing utility for building and running custom quantum circuits.

## Usage

In Quemu, a user constructs an instance of `CircuitBuilder`, which can be used to incrementally build a quantum circuit by adding instances of `Gate`.  The user then calls `CircuitBuilder::Get` to retrieve a finished `Circuit` instance.

```c++
auto builder = CircuitBuilder();
auto circuit = builder.BuildCircuit()
                     .AddGateX(1, 0)
                     .AddGateY(2, 0)
                     .Get();
```

An instance of `Circuit` is meant to act on a reference to an instance of `State`, through the `Circuit::Transform` method.  Before the transform, the state  belongs to the input register of the circuit, whereas the resulting state belongs to the output register of the circuit.

```c++
const auto num_qubits = 4;
auto state = State(num_qubits);
const auto success = circuit->Transform(state);
```


