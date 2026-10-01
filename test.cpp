#include "output.h"
#include <iostream>

int main() {
    CSVOutput out("test.csv");
    SimulationResult result;
    for (int i = 0; i < 5; i++) {              // made-up numbers
        out.record(i * 0.01, 100, i * 20, 5);
        result.addReading(i * 20, 100);
    }
    std::cout << "Steady-state error: " << result.steadyStateError() << "\n";
}
