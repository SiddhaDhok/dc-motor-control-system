# Closed-Loop DC Motor Controller

A simulation of closed-loop speed control for a DC motor, using a PID
(Proportional-Integral-Derivative) controller to drive a simulated motor to a
target speed and hold it there.

## What this does

The program simulates a motor and a controller working together in a
feedback loop: the controller measures the motor's current speed, compares
it to a target speed, and computes a corrective power output using a
combination of three terms (P, I, D). That power is applied to the motor,
which updates its speed accordingly, and the cycle repeats until the motor
settles at the target speed. All readings are logged to a CSV file for
plotting in Excel.

## Project structure
dc-motor-control-system/
├── main.cpp # Entry point — takes user input, runs the simulation
├── MotorController.h # Controller and motor classes (PID logic + motor physics)
├── output.h # Output, CSVOutput, and SimulationResult classes
└── README.md


## Classes

| Class | Responsibility |
|---|---|
| `Controller` | Holds Kp, Ki, Kd; computes the PID control signal from target vs. measured speed |
| `motor` | Inherits from `Controller`; models speed and inertia, and runs the control loop until the target speed is reached |
| `Output` | Abstract base class for recording simulation readings |
| `CSVOutput` | Subclass of `Output`; writes each reading (time, target, speed, power) to a CSV file |
| `SimulationResult` | Stores the full run's readings and computes summary metrics (peak speed, overshoot %, settling time, steady-state error) |

## Build and run

Requires a C++17-capable compiler (`g++` via WSL, MinGW, or similar).

```bash
g++ -std=c++17 main.cpp -o sim_demo
./sim_demo          # on WSL/Linux
# or
sim_demo.exe        # on native Windows
```

## Usage

The program prompts for the following inputs at runtime:

| Input | Description | Example |
|---|---|---|
| Target speed | The speed the motor should reach | `20` |
| Motor inertia | Resistance to changing speed — higher values respond more slowly | `5` |
| Kp, Ki, Kd | PID gains, space-separated | `2 0.5 0.5` |
| dt | Time step between measurements (seconds) | `0.01` |

**Note:** the simulation runs until the motor settles within 0.01 of the
target speed — it does not run for a fixed duration. If the chosen gains are
too weak relative to the inertia, the loop may take a long time to converge
(or appear to hang) — press `Ctrl+C` to stop it and try different values.

## Output

After each run, the program:
- Prints live speed values to the console as the motor approaches target
- Writes every reading to `sim_results.csv` (columns: `time`, `target`, `speed`, `power`)
- Prints a summary: peak speed, overshoot %, settling time, and steady-state error

### Plotting in Excel
1. Open `sim_results.csv` in Excel
2. Select the `time`, `target`, and `speed` columns
3. Insert → Line Chart
4. This shows the motor's speed climbing toward (and settling at) the target

## Team / Roles

| Role | Owns |
|---|---|
| Motor & Controller | `MotorController.h` |
| Output & Results | `output.h` |
| Simulation Driver & Integration | `main.cpp` |

## Status

- [x] PID controller implemented (P, I, D combined)
- [x] Motor physics model (speed, inertia)
- [x] CSV export for plotting
- [x] Summary metrics (overshoot, settling time, steady-state error)
