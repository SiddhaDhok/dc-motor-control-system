#pragma once
#include <bits/stdc++.h>

class motor {
private:
    double speed;
    double inertia;

    double SR(double x) {
        if (x >= 0) return std::sqrt(x);
        return 0 - std::sqrt(-x);
    }
    double NewSpeed(double P, double dt) {
        double s2  = speed * speed;
        double DE  = (2 * P * dt) / inertia;
        double NS2 = s2 + DE;
        if (NS2 < 0) return 0;
        return SR(NS2);
    }

public:
    motor(double m) : speed(0), inertia(m) {}

    void update(double power, double dt) {   // replaces ChangeSpeed()'s inner step
        speed = NewSpeed(power, dt);
    }
    double GetSpeed()   { return speed; }
    double GetInertia() { return inertia; }
};
