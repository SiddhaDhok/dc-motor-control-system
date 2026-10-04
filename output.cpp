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
        for (int i = 0; i < speeds.size(); i++)
            if (fabs(speeds[i] - target) > 0.02 * target)
                result = times[i];
        return result;
    }

    double steadyStateError() {
        return target - speeds.back();
    }
};

int main() {
    CSVOutput out;
    SimulationResult result;

    double target = 100;
    for (int i = 0; i <= 1000; i++) {
        double t = i * 0.01;
        double speed = target * (1 - exp(-3 * t) * cos(6 * t));
        double power = 5 * (target - speed) / target;
        out.record(t, target, speed, power);
        result.addReading(t, target, speed);
    }

    cout << "Overshoot: " << result.overshoot() << " %" << endl;
    cout << "Settling time: " << result.settlingTime() << " s" << endl;
    cout << "Steady state error: " << result.steadyStateError() << endl;
    return 0;
}
