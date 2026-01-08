# Object-Oriented Programming | Autonomous Driving Simulation

**Name:** Panagiotis Christoforidis (Παναγιώτης Χριστοφορίδης)

**sdi:** sdi2400246 (R/N: 1115202400246)

**Repository:** [mrgrewk/OPP-Project](https://github.com/mrgrewk/OPP-Project)

**Language:** C++

This repository contains a simplified simulation of a **fully autonomous self-driving car** navigating a 2D grid world using **GPS targets**, **sensor perception**, and **sensor fusion**.  
The project is designed to demonstrate core **Object-Oriented Programming (OOP)** principles in practice.

> **When I wrote this simulation, I thought I understood autonomous driving.**  
> **Now… the car understands more than I do.**

## Project Overview

The simulation models a **GridWorld** populated with static and moving objects.  
A **SelfDrivingCar** perceives its environment through multiple sensors, fuses the sensor data, and makes navigation decisions to reach one or more GPS targets.

Key concepts demonstrated:
- Encapsulation
- Inheritance
- Polymorphism
- Composition
- Object communication (messaging)

## Main Components

### 1. **GridWorld**
A 2D grid environment with configurable dimensions (default `40x40`).

Responsibilities:
- Stores and manages all world objects
- Updates object states every simulation tick
- Handles visualization (full map & point-of-view)

---

### 2. **SelfDrivingCar**
The autonomous agent of the simulation.

Features:
- Discrete speed states: `STOPPED`, `HALF_SPEED`, `FULL_SPEED`
- Directional movement and turning
- Acceleration and deceleration logic
- GPS-based navigation
- Boundary and goal detection

The car:
1. Collects sensor data  
2. Fuses measurements  
3. Makes navigation decisions  
4. Executes movement  

### 3. **Sensors**
Each sensor has different capabilities, range, and accuracy.

Implemented sensor types:
- **Lidar** – 360° perception, high distance accuracy
- **Radar** – Detects moving objects, returns speed and direction
- **Camera** – Detects object types, traffic light colors, and sign text

Each sensor produces `SensorReading` objects with a confidence score.

### 4. **Sensor Fusion Engine**
Combines multiple sensor readings into a single, unified perception.

Key ideas:
- Group readings by object ID
- Use confidence-weighted averaging
- Apply minimum confidence threshold
- Special safety handling for bicycles

### 5. **World Objects**

#### Static Objects
- Parked Cars (`P`)
- Stop Signs (`S`)
- Traffic Lights (`R`, `Y`, `G`)

#### Moving Objects
- Cars (`C`)
- Bikes (`B`)

Each object has:
- Unique ID
- Position
- Glyph for visualization

## Visualization

Two visualization modes are supported:

### Full Map
- Printed at simulation start and end
- Displays the entire grid world

### POV (Point of View)
- Printed every tick
- Shows a radius around the autonomous car

**Glyph Priority (highest → lowest):**

| Object | Symbol |
|------|--------|
| Self-driving car | `@` |
| Empty cell | `.` |
| Outside bounds | `X` |
| Red light | `R` |
| Yellow light | `Y` |
| Stop sign | `S` |
| Moving bike | `B` |
| Moving car | `C` |
| Green light | `G` |
| Parked car | `P` |
| Unknown | `?` |

## Build Instructions

Compile the project using `make`:

```bash
make
```

Clean build artifacts:

```bash
make clean
```

Compiler settings:
- `g++`
- `std=c++17`
- `Wall -Wextra -g`

## Running the Simulation
The simulation is fully configurable via command-line arguments.

## Required
- At least one GPS target using `--gps`

## Example

```bash
./oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15
```

## Command-Line Options

| Option | Description | Default |
|------|------------|---------|
| `--seed <int>` | Random seed | current time |
| `--dimX <int>` | World width | 40 |
| `--dimY <int>` | World height | 40 |
| `--numMovingCars <int>` | Number of moving cars | 3 |
| `--numMovingBikes <int>` | Number of moving bikes | 4 |
| `--numParkedCars <int>` | Number of parked cars | 5 |

## Simulation Termination
The simulation ends when:
- Maximum ticks are reached, or
- The car moves outside world bounds, or
- All GPS targets are successfully reached

## Methodology
- **Key classes & primary fields**
	- `GridWorld`: dimensions, list of `WorldObject` pointers, tick counter
	- `WorldObject` / `StaticObject` / `MovingObject`: `id`, `position` (`x,y`), `glyph`, behavior methods
	- `SelfDrivingCar`: `position`, `heading`, `speedState` (`STOPPED`/`HALF_SPEED`/`FULL_SPEED`), GPS target queue, sensors
	- `SensorReading`: `objectId`, measured `position`, optional `speed`/`heading`, `confidence`
	- `SensorFusionEngine`: accepts `SensorReading`s and outputs fused object estimates

- **Assumptions**
	- Simulation advances in discrete ticks; the user advances each tick by pressing ENTER.
	- The car begins at `(0,0)` and GPS targets are provided as pairs via `--gps x y [x y ...]`.
	- Objects cannot occupy the same exact grid cell when spawned (placement attempts are retried).
	- Moving-object collision handling is simplified: objects avoid overlapping positions; the self-driving car prioritizes safety.

- **Sensor models (high level)**
	- `Lidar`: 360° coverage, high distance accuracy; returns positions with small Gaussian noise.
	- `Radar`: detects moving objects and reports relative speed and heading with moderate accuracy.
	- `Camera`: identifies object types and traffic light states (color) with variable confidence; reads stop sign text.
	- Each sensor returns a `confidence` in [0,1]; lower confidence increases measurement variance.

- **Sensor fusion algorithm**
	1. Group incoming `SensorReading`s by `objectId`.
	2. Discard readings below a minimum confidence threshold (default ~0.25).
	3. Compute confidence-weighted averages for position (separately for `x` and `y`):

		 $$x_f = \frac{\sum_i c_i x_i}{\sum_i c_i},\quad y_f = \frac{\sum_i c_i y_i}{\sum_i c_i}$$

		 where $c_i$ is the confidence of reading $i$.
	4. For velocity or categorical attributes (e.g., light color), use highest-confidence reading or majority weighted by confidence.
	5. Apply safety heuristics (e.g., give extra distance buffer for bicycles).

- **Navigation & control**
	- GPS targets are stored in a queue; the current target is the head of the queue.
	- The car computes a simple heading toward the target and chooses turn/forward actions to reduce Manhattan distance while obeying speed/state transitions.
	- Speed transitions follow simple rules: accelerate toward `FULL_SPEED` when the path is clear, decelerate or stop for red lights, stop signs, or nearby obstacles.
	- Traffic lights and stop signs are obeyed using sensor-fused estimates and confidence thresholds.

- **World setup & randomness**
	- Objects are placed randomly using a RNG seeded by `--seed` for reproducibility.
	- Default entity counts are used unless overridden by flags.

## Notes
- Object placement is randomized but reproducible with `--seed`
- The car always starts at `(0,0)`
- Press **ENTER** to advance each simulation tick
- Logging can be extended to file output if desired
