#ifndef CONTROLLER_H
#define CONTROLLER_H

class Motor;

class Gain {
protected:
    double k;
public:
    Gain(double k);
    virtual double apply(double error, double dt) = 0;
    virtual ~Gain() {}
};

class ProportionalGain : public Gain {
public:
    ProportionalGain(double k);
    double apply(double error, double dt);
};

class IntegralGain : public Gain {
private:
    double sum;
public:
    IntegralGain(double k);
    double apply(double error, double dt);
};

class DerivativeGain : public Gain {
private:
    double prevError;
    bool first;
public:
    DerivativeGain(double k);
    double apply(double error, double dt);
};

class Controller {
protected:
    Motor& motor;

    double lastError, lastP, lastI, lastD, lastPower;
    double measureError(double target);

public:
    Controller(Motor& motor);
    virtual double step(double target, double dt) = 0;
    virtual ~Controller() {}

    double getError();
    double getP();
    double getI();
    double getD();
    double getPower();
};

// Only Kp
class PController : public Controller {
private:
    ProportionalGain p;
public:
    PController(double Kp, Motor& motor);
    double step(double target, double dt);
};

// Only Ki
class IController : public Controller {
private:
    IntegralGain i;
public:
    IController(double Ki, Motor& motor);
    double step(double target, double dt);
};

// Only Kd
class DController : public Controller {
private:
    DerivativeGain d;
public:
    DController(double Kd, Motor& motor);
    double step(double target, double dt);
};

// Ki and Kd
class IDController : public Controller {
private:
    IntegralGain i;
    DerivativeGain d;
public:
    IDController(double Ki, double Kd, Motor& motor);
    double step(double target, double dt);
};

// Kp and Ki
class PIController : public Controller {
private:
    ProportionalGain p;
    IntegralGain i;
public:
    PIController(double Kp, double Ki, Motor& motor);
    double step(double target, double dt);
};

// Kp and Kd
class PDController : public Controller {
private:
    ProportionalGain p;
    DerivativeGain d;
public:
    PDController(double Kp, double Kd, Motor& motor);
    double step(double target, double dt);
};

// Kp, Ki and Kd
class PIDController : public Controller {
private:
    ProportionalGain p;
    IntegralGain i;
    DerivativeGain d;
public:
    PIDController(double Kp, double Ki, double Kd, Motor& motor);
    double step(double target, double dt);
};

#endif