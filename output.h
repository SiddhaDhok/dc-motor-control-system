#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class Output {
public:
    virtual void record(double time, double target, double speed, double power) {}
};

class CSVOutput : public Output {
    ofstream file;
public:
    CSVOutput() {
        file.open("results.csv");
        file << "time,target,speed,power\n";
    }
    void record(double time, double target, double speed, double power) {
        file << time << "," << target << "," << speed << "," << power << "\n";
    }
};

class SimulationResult {
    vector<double> times;
    vector<double> speeds;
    double target = 0;
public:
    void addReading(double time, double tgt, double speed) {
        times.push_back(time);
        speeds.push_back(speed);
        target = tgt;
    }

    double overshoot() {
        double peak = 0;
        for (double s : speeds)
            if (s > peak) peak = s;
        if (peak <= target) return 0;
        return (peak - target) / target * 100;
    }

    double settlingTime() {
        double result = 0;
        for (size_t i = 0; i < speeds.size(); i++)
            if (fabs(speeds[i] - target) > 0.02 * target)
                result = times[i];
        return result;
    }

    double steadyStateError() {
        return target - speeds.back();
    }
};
