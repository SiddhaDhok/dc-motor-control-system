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
    CSVOutput(string filename = "results.csv");
    ~CSVOutput();
    void record(double time, double target, double speed, double power) override;
};

class SimulationResult {
    vector<double> times;
    vector<double> targets;
    vector<double> speeds;
    vector<double> powers;
public:
    void addReading(double time, double target, double speed, double power = 0);
    int count();
    double peakSpeed();
    double overshoot();
    double settlingTime();
    double steadyStateError();
    void print();
};
