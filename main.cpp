#include "Motor.h"
#include "Controller.h"
#include "output.h"

int main() {
    double target = 400;
    double dt = 0.01;

    motor m(1.0);
    Controller c(12.0, 6.0, 0.0);
    CSVOutput out("results.csv");
    SimulationResult result;

    for (int i = 0; i <= 1000; i++) {
        double t = i * dt;
        double power = c.step(target, m.GetSpeed(), dt);
        m.update(power, dt);
        double speed = m.GetSpeed();
        out.record(t, target, speed, power);
        result.addReading(t, target, speed, power);
    }

    result.print();
    return 0;
}
