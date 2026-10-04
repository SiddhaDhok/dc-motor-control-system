#pragma once

class Gain {
protected:
    double k;
public:
    Gain(double k) : k(k) {}
    virtual double apply(double error, double dt) = 0;
    virtual ~Gain() {}
};

class ProportionalGain : public Gain {
public:
    ProportionalGain(double Kp) : Gain(Kp) {}
    double apply(double error, double dt) {
        return k * error;
    }
};

class IntegralGain : public Gain {
private:
    double sum;
public:
    IntegralGain(double Ki) : Gain(Ki), sum(0.0) {}
    double apply(double error, double dt) {
        sum += error * dt;
        return k * sum;
    }
};

class DerivativeGain : public Gain {
private:
    double prevError;
public:
    DerivativeGain(double Kd) : Gain(Kd), prevError(0.0) {}
    double apply(double error, double dt) {
        double rate = 0.0;
        rate = (error - prevError) / dt;
        prevError = error;
        return k * rate;
    }
};

class Controller {
private:
    double Kp, Ki, Kd;
    Gain* pGain;
    Gain* iGain;
    Gain* dGain;

public:
    Controller(double Kp, double Ki, double Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {
        pGain = new ProportionalGain(Kp);
        iGain = new IntegralGain(Ki);
        dGain = new DerivativeGain(Kd);
    }
    ~Controller() { delete pGain; delete iGain; delete dGain; }
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    double step(double target, double speed, double dt) {
        double error = target - speed;
        return pGain->apply(error, dt)
             + iGain->apply(error, dt)
             + dGain->apply(error, dt);
    }
};
