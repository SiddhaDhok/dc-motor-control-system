#include "output.h"

// ---------- CSVOutput ----------
CSVOutput::CSVOutput(string filename) {
    file.open(filename);
    if (!file.is_open()) {
        cout << "Could not open " << filename << endl;
        return;
    }
    file << "time,target,speed,power\n";
}

CSVOutput::~CSVOutput() {
    if (file.is_open()) file.close();
}

void CSVOutput::record(double time, double target, double speed, double power) {
    if (file.is_open())
        file << time << "," << target << "," << speed << "," << power << "\n";
}

// ---------- SimulationResult ----------
void SimulationResult::addReading(double time, double target, double speed, double power) {
    times.push_back(time);
    targets.push_back(target);
    speeds.push_back(speed);
    powers.push_back(power);
}

int SimulationResult::count() {
    return times.size();
}

double SimulationResult::peakSpeed() {
    double peak = 0;
    for (double s : speeds)
        if (s > peak) peak = s;
    return peak;
}

double SimulationResult::overshoot() {
    if (speeds.empty() || targets.back() == 0) return 0;
    double target = targets.back();
    double peak = peakSpeed();
    if (peak <= target) return 0;
    return (peak - target) / target * 100;
}

double SimulationResult::settlingTime() {
    if (speeds.empty()) return 0;
    double target = targets.back();
    double result = 0;
    for (size_t i = 0; i < speeds.size(); i++)
        if (fabs(speeds[i] - target) > 0.02 * fabs(target))
            result = times[i];
    return result;
}

double SimulationResult::steadyStateError() {
    if (speeds.empty()) return 0;
    return targets.back() - speeds.back();
}

void SimulationResult::print() {
    cout << "Readings: " << count() << endl;
    cout << "Peak speed: " << peakSpeed() << endl;
    cout << "Overshoot: " << overshoot() << " %" << endl;
    cout << "Settling time: " << settlingTime() << " s" << endl;
    cout << "Steady state error: " << steadyStateError() << endl;
}
