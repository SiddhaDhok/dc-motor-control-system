#include "MotorController.h"
#include "output.h"

int main() {
    double target = 100;
    double dt = 0.01;

    motor m(1.0, 0.01, 0.0, 3.0, dt);
    CSVOutput out;
    SimulationResult result;

    for (int i = 0; i <= 1000; i++) {
        double t = i * dt;
        double power = m.Step(target);
        double speed = m.GetSpeed();
        out.record(t, target, speed, power);
        result.addReading(t, target, speed);
    }

    cout << "Overshoot: " << result.overshoot() << " %" << endl;
    cout << "Settling time: " << result.settlingTime() << " s" << endl;
    cout << "Steady state error: " << result.steadyStateError() << endl;
    return 0;
}
