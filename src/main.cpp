#include "Executor.hpp"

int main(int argc, char const* argv[]) {
  if (2 == argc) {
    std::ifstream input(argv[1]);
    Executor e(input);
    auto solution = e.solve();
    if (solution) {
      solution->print_steps();
    }
  }
}
