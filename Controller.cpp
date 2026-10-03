#include <bits/stdc++.h>
using namespace std;

class Comparator {
public:
    double compare(double target_s, double measured_s){
        return target_s - measured_s;
    }
};

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
    ProportionalGain(double k) : Gain(k) {}
    double apply(double error, double dt) { 
        return k * error; 
    }
};

class IntegralGain : public Gain {
private:
    double sum, sumLimit;
public:
    IntegralGain(double k, double outputLimit) : Gain(k), sum(0.0) {
        sumLimit = (k > 0) ? outputLimit / k : 0.0;
    }
    double apply(double error, double dt) {
        sum += error * dt;
        if (sum > sumLimit)  sum = sumLimit;
        if (sum < -sumLimit) sum = -sumLimit;
        return k * sum;
    }
};

class DerivativeGain : public Gain {
private:
    double prevError;
    bool first;
public:
    DerivativeGain(double k) : Gain(k), prevError(0.0), first(true) {}
    double apply(double error, double dt) {
        double rate = 0.0;
        if (!first) rate = (error - prevError) / dt;
        prevError = error;
        first = false;
        return k * rate;
    }
};