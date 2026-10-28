#include "Controller.h"
#include "Motor.h"

// ---------- Gain ----------
Gain::Gain(double k) : k(k) {}

// ---------- ProportionalGain ----------
ProportionalGain::ProportionalGain(double k) : Gain(k) {}

double ProportionalGain::apply(double error, double dt) {
    return k * error;
}

// ---------- IntegralGain ----------
IntegralGain::IntegralGain(double k) : Gain(k), sum(0.0) {}

double IntegralGain::apply(double error, double dt) {
    sum += error * dt;
    return k * sum;
}

// ---------- DerivativeGain ----------
DerivativeGain::DerivativeGain(double k) : Gain(k), prevError(0.0), first(true) {}

double DerivativeGain::apply(double error, double dt) {
    double rate = 0.0;
    if (!first) rate = (error - prevError) / dt;
    prevError = error;
    first = false;
    return k * rate;
}

// ---------- Controller (base) ----------
Controller::Controller(Motor& motor)
    : motor(motor), lastError(0.0), lastP(0.0), lastI(0.0), lastD(0.0), lastPower(0.0) {}

double Controller::measureError(double target) {
    double measured = motor.getSpeed();
    lastError = target - measured;
    return lastError;
}

double Controller::getError() { return lastError; }
double Controller::getP() { return lastP; }
double Controller::getI() { return lastI; }
double Controller::getD() { return lastD; }
double Controller::getPower() { return lastPower; }

// ---------- PController ----------
PController::PController(double Kp, Motor& motor)
    : Controller(motor), p(Kp) {}

double PController::step(double target, double dt) {
    double error = measureError(target);
    lastP = p.apply(error, dt);
    lastPower = lastP;
    return lastPower;
}

// ---------- IController ----------
IController::IController(double Ki, Motor& motor)
    : Controller(motor), i(Ki) {}

double IController::step(double target, double dt) {
    double error = measureError(target);
    lastI = i.apply(error, dt);
    lastPower = lastI;
    return lastPower;
}

// ---------- DController ----------
DController::DController(double Kd, Motor& motor)
    : Controller(motor), d(Kd) {}

double DController::step(double target, double dt) {
    double error = measureError(target);
    lastD = d.apply(error, dt);
    lastPower = lastD;
    return lastPower;
}

// ---------- PIController ----------
PIController::PIController(double Kp, double Ki, Motor& motor)
    : Controller(motor), p(Kp), i(Ki) {}

double PIController::step(double target, double dt) {
    double error = measureError(target);
    lastP = p.apply(error, dt);
    lastI = i.apply(error, dt);
    lastPower = lastP + lastI;
    return lastPower;
}

// ---------- PDController ----------
PDController::PDController(double Kp, double Kd, Motor& motor)
    : Controller(motor), p(Kp), d(Kd) {}

double PDController::step(double target, double dt) {
    double error = measureError(target);
    lastP = p.apply(error, dt);
    lastD = d.apply(error, dt);
    lastPower = lastP + lastD;
    return lastPower;
}

// ---------- IDController ----------
IDController::IDController(double Ki, double Kd, Motor& motor)
    : Controller(motor), i(Ki), d(Kd) {}

double IDController::step(double target, double dt) {
    double error = measureError(target);
    lastI = i.apply(error, dt);
    lastD = d.apply(error, dt);
    lastPower = lastI + lastD;
    return lastPower;
}

// ---------- PIDController ----------
PIDController::PIDController(double Kp, double Ki, double Kd, Motor& motor)
    : Controller(motor), p(Kp), i(Ki), d(Kd) {}

double PIDController::step(double target, double dt) {
    double error = measureError(target);
    lastP = p.apply(error, dt);
    lastI = i.apply(error, dt);
    lastD = d.apply(error, dt);
    lastPower = lastP + lastI + lastD;
    return lastPower;
}