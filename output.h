#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

class Output {
public:
    virtual ~Output() {}
    virtual void record(double time, double target, double speed, double power) = 0;
};

class CSVOutput : public Output {
    ofstream file;
public:
    CSVOutput(string filename = "results.csv") {
        file.open(filename);
        if (!file.is_open()) {
            cout << "Could not open " << filename << endl;
            return;
        }
        file << "time,target,speed,power\n";
    }
    ~CSVOutput() {
        if (file.is_open()) file.close();
    }
    void record(double time, double target, double speed, double power) override {
        if (file.is_open())
            file << time << "," << target << "," << speed << "," << power << "\n";
    }
};

class SimulationResult {
    vector<double> times;
    vector<double> targets;
    vector<double> speeds;
    vector<double> powers;
public:
    void addReading(double time, double target, double speed, double power = 0) {
        times.push_back(time);
        targets.push_back(target);
        speeds.push_back(speed);
        powers.push_back(power);
    }

    int count() {
        return times.size();
    }

    double peakSpeed() {
        double peak = 0;
        for (double s : speeds)
            if (s > peak) peak = s;
        return peak;
    }

    double overshoot() {
        if (speeds.empty() || targets.back() == 0) return 0;
        double target = targets.back();
        double peak = peakSpeed();
        if (peak <= target) return 0;
        return (peak - target) / target * 100;
    }

    double settlingTime() {
        if (speeds.empty()) return 0;
        double target = targets.back();
        double result = 0;
        for (size_t i = 0; i < speeds.size(); i++)
            if (fabs(speeds[i] - target) > 0.02 * fabs(target))
                result = times[i];
        return result;
    }

    double steadyStateError() {
        if (speeds.empty()) return 0;
        return targets.back() - speeds.back();
    }

    void print() {
        cout << "Readings: " << count() << endl;
        cout << "Peak speed: " << peakSpeed() << endl;
        cout << "Overshoot: " << overshoot() << " %" << endl;
        cout << "Settling time: " << settlingTime() << " s" << endl;
        cout << "Steady state error: " << steadyStateError() << endl;
    }
};
