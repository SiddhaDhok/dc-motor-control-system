#include "MotorController.h"
#include "output.h"

int main() {
    double target = 400;
    double dt = 0.01;

    motor m(1.0, 0.06, 0.0, 12.0, dt);
    CSVOutput out("results.csv");
    SimulationResult result;

    for (int i = 0; i <= 1000; i++) {
        double t = i * dt;
        double power = m.Step(target);
        double speed = m.GetSpeed();
        out.record(t, target, speed, power);
        result.addReading(t, target, speed, power);
    }

    result.print();
    return 0;
}
