#include <iostream>
#include <fstream>
using namespace std;

class Output {
public:
    virtual void record(double time, double speed) {}
};

class CSVOutput : public Output {
    ofstream file;
public:
    CSVOutput() { file.open("results.csv"); }
    void record(double time, double speed) { file << time << "," << speed << "\n"; }
};

class SimulationResult {
public:
    double overshoot = 0;
};

int main() {
    CSVOutput out;
    out.record(0.01, 20);
    cout << "started" << endl;
}
