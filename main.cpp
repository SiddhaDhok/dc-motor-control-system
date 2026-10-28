#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include "Controller.h"
#include "Motor.h"
#include "output.h"
#include "MultithreadSimulation.h"   // change to your header's real name if it differs
using namespace std;

// Builds the controller type asked for. Returns nullptr if the name is unknown.
// The gains are the values we tested; change them here.
unique_ptr<Controller> makeController(const string& type, Motor& motor) {
	const double Kp = 1.5, Ki = 0.65, Kd = 0.75;

	if (type == "p")   return make_unique<PController>(Kp, motor);
	if (type == "i")   return make_unique<IController>(Ki, motor);
	if (type == "d")   return make_unique<DController>(Kd, motor);
	if (type == "pi")  return make_unique<PIController>(Kp, Ki, motor);
	if (type == "pd")  return make_unique<PDController>(Kp, Kd, motor);
	if (type == "id")  return make_unique<IDController>(Ki, Kd, motor);
	if (type == "pid") return make_unique<PIDController>(Kp, Ki, Kd, motor);
	return nullptr;
}

int main(int argc, char* argv[]){
	const double target = 30;
	const double dt = 1;

	// Usage: ./sim [single|threaded] [p|i|d|pi|pd|id|pid] [run number]
	string mode = (argc > 1) ? argv[1] : "single";
	string type = (argc > 2) ? argv[2] : "pid";
	string run  = (argc > 3) ? argv[3] : "";
	bool threaded = (mode == "threaded");

	if (mode != "single" && mode != "threaded") {
		cout << "Usage: ./sim [single|threaded] [p|i|d|pi|pd|id|pid] [run number]" << endl;
		return 1;
	}

	Motor motor(50);
	unique_ptr<Controller> controller = makeController(type, motor);
	if (!controller) {
		cout << "Unknown controller '" << type << "'. Use p, i, d, pi, pd, id or pid." << endl;
		return 1;
	}

	string csvName = "sim_results_" + mode + "_" + type + (run.empty() ? "" : "_" + run) + ".csv";
	cout << "Mode: " << mode << ", controller: " << type << ", output: " << csvName << endl;

	if (!threaded) {
		const double tol = 0.02 * target;   // same +-2% band as settlingTime()
		const int holdSteps = 20;
		const int maxSteps = 5000;          // safety cap

		CSVOutput csv(csvName);
		SimulationResult result;

		int settledCount = 0;
		bool settled = false;

		for (int k = 0; k < maxSteps; k++) {
			double t = k * dt;
			double p = controller->step(target, dt);
			motor.setPower(p);
			motor.update(dt);
			double s = motor.getSpeed();
			csv.record(t, target, s, p);
			result.addReading(t, target, s, p);

			if (fabs(target - s) <= tol) settledCount++;
			else settledCount = 0;
			if (settledCount >= holdSteps) { settled = true; break; }
		}

		if (settled) cout << "Stopped: speed stayed within +-2% for " << holdSteps << " steps" << endl;
		else         cout << "Stopped: reached the " << maxSteps << "-step cap without settling" << endl;
		result.print();
	} else {
		CSVOutput csv(csvName);
		SimulationResult result;

		ThreadedSimulation sim(motor, *controller, csv, result, target, dt);
		bool settled = sim.run();

		if (settled) cout << "Stopped: speed stayed within +-2% (threaded)" << endl;
		else         cout << "Stopped: reached the sample cap without settling (threaded)" << endl;
		result.print();
	}
	return 0;
}
