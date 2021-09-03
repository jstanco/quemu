#include <cassert>

#include "quemu/quemu.h"

namespace quemu {

int Main() {
  auto builder = CircuitBuilder();
  auto circuit = builder.BuildCircuit()
                     .AddGateX(1, 0)
                     .AddGateY(2, 0)
                     .AddGateZ(3, 0)
                     .AddGateX(1, 1)
                     .Get();

  auto state = State(4);

  assert(circuit->Apply(state));
  return 0;
}

}  // namespace quemu

int main() { return quemu::Main(); }
