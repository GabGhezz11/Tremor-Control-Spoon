# Description

An electronic Tremor Control Spoon designed to compensate involuntary hand
tremors in Parkinson's disease patients during meals, developed as part of the
Biodesign course at Università Campus Bio-Medico di Roma in 2023 by a team of 10 students.

---

## Overview

This project focused on the design and development of an electronically
stabilized spoon to assist individuals with Parkinson's disease by compensating
tremors during meal consumption, promoting greater independence and comfort in
daily life. The work was developed in collaboration with a team of colleagues,
with my contribution made remotely while based in Vienna.

The device integrates mechanical, electronic, and control system design:

- **Sensing**: an IMU sensor (MPU-6050) for real-time motion tracking
  (acceleration and angular velocity)
- **Control**: an ATMega328-MUR microcontroller processes sensor data through a
  complementary filter to estimate roll and pitch angles
- **Actuation**: dual servo motors compensate roll and pitch tremors in real
  time, with compensation limited to ±30° on both axes to preserve natural
  eating motion

## Repository Structure

- `firmware/` — Arduino/C firmware implementing the sense-plan-act control loop
  - `TCS_control.ino` — main Arduino sketch
  - `Functions.c` — control and filtering functions
  - `Variables.h` — shared variable/constant definitions
- `electronics/` — PCB design and documentation (Eagle CAD)
  - `scheda_progetto_ver_4.2.sch` — schematic
  - `scheda_progetto_ver_4.2.brd` — board layout
  - `BoM_ver4.2.xlsx` — bill of materials
  - `scheda_progetto_ver_4` — project/design documentation
- `cad/` — Mechanical design files (Solid Edge/STEP) for the handle, motor
  case, and spoon tool
  - `.step Files/` — final STEP files, importable in any CAD software
  - `Drafts/` — intermediate/working Solid Edge design iterations
- `data-analysis/` — MATLAB scripts used to extract tremor frequency and
  acceleration/angular velocity ranges from a reference Parkinsonian tremor
  dataset, informing sensor and motor specifications

## Design Highlights

**Mechanical**: modeled in Solid Edge and based on anthropometric hand data
from adult users, optimized for usability, compactness, and dishwasher-safe
cleaning. Made of polypropylene (biocompatible, heat-resistant, dishwasher-safe)
with silicon caps for the removable USB/charging ports.

**Electronics**: sensing, control, and power supply modules integrated onto a
single miniaturized PCB (57 x 31 mm, a 10% area reduction from the original
demo stripboard), powered by a 2S1P LiPo battery (800 mAh, 7.4 V), with voltage
regulation and USB communication circuits.

**Firmware**: a complementary filter combines accelerometer and gyroscope data
to estimate roll and pitch angles, driving the servos to counteract tremor in
real time while allowing intentional tilting (e.g., to scoop food).

## Personal Contribution - Matlab Data Analysis

The analysis focused on two tremor-related tasks from the reference dataset:

- **Kinetic tremor (task AS316)**: analyzed to extract the acceleration and
  angular velocity ranges during a spoon-eating-like motion.
- **Postural tremor (task AS315)**: analyzed to identify the maximum and
  dominant frequencies of the Parkinsonian tremor signal.

### Scripts

| Script | Purpose |
|---|---|
| `Ranges_acc_v.mlx` | Computes the acceleration and angular velocity **ranges** (kinetic tremor, all 3 subjects, all 3 sensor locations) |
| `Dominant_freq_subj1.mlx` | Computes the **dominant frequency** of acc/gyro signals for Subject 1 (postural tremor) |
| `Dominant_freq_subj2.mlx` | Computes the **dominant frequency** of acc/gyro signals for Subject 2 (postural tremor) |
| `Dominant_freq_subj3.mlx` | Computes the **dominant frequency** of acc/gyro signals for Subject 3 (postural tremor) |
| `Dominant_freq_global.mlx` | Aggregates the dominant-frequency results of all subjects/locations and returns the global maximum |
| `Max_freq_subj1.mlx` | Computes the **maximum frequency** (95% PSD energy threshold) for Subject 1 (postural tremor) |
| `Max_freq_subj2.mlx` | Computes the **maximum frequency** (95% PSD energy threshold) for Subject 2 (postural tremor) |
| `Max_freq_subj3.mlx` | Computes the **maximum frequency** (95% PSD energy threshold) for Subject 3 (postural tremor) |
| `Max_freq_global.mlx` | Aggregates the maximum-frequency results of all subjects/locations and returns the global maximum |
| `createFit.m` | Auto-generated helper (MATLAB Distribution Fitter) used to fit a probability distribution to the angular velocity data — required by `Ranges_acc_v.mlx` |

### Methodology

**1. Signal preprocessing**
- Raw 3-axis accelerometer and gyroscope data are extracted from the dataset (`Subj{i}.off.taskASxxx.<sensor_location>.acc/.gyro`).
- A 4th-order Butterworth **high-pass filter** (cut-off 1 Hz), applied with `filtfilt` (zero-phase), removes gravity/motion bias while preserving tremor content.
- The 3-axis signals are combined into a single magnitude signal via the Euclidean norm (vector module).

**2. Frequency-domain analysis (postural tremor)**
- The **Power Spectral Density (PSD)** of the filtered signal is estimated with `periodogram`.
- **Dominant frequency**: frequency corresponding to the PSD peak.
- **Maximum frequency**: frequency below which 95% of the cumulative PSD energy is contained (computed via `trapz` on the PSD curve).
- Both metrics are computed per subject, per sensor location (index, thumb, metacarpus), and per signal type (acceleration, angular velocity), then aggregated across subjects to obtain the **global maximum/dominant frequency**, used to define the minimum IMU sampling rate (Nyquist criterion).

**3. Amplitude analysis (kinetic tremor)**
- Acceleration and angular velocity magnitudes are pooled across all subjects and sensor locations.
- A **kernel density estimate** (`ksdensity`, positive support) is computed for both signals.
- The upper bound of the working range is defined as the value below which 95% of the cumulative probability density falls; the range is then obtained by mirroring this value onto the negative axis (assuming symmetric bidirectional motion).
- The result is cross-checked against the 95th percentile/quantile of the raw pooled data (`quantile`, `prctile`) as a validation step.
- `createFit.m` is used to verify that the angular velocity distribution is **not normal**, justifying the use of the empirical (KDE-based) approach instead of a parametric one.

### Outputs used for hardware design
- **Acceleration range** → used to select the IMU accelerometer full-scale range.
- **Angular velocity range** → used to select the IMU gyroscope full-scale range.
- **Global maximum/dominant frequency** → used to define the minimum IMU sampling rate (via Nyquist criterion) and to size the servo motor bandwidth/response requirements.

Results informed the electronic design directly: a maximum signal frequency of 42 Hz and dominant frequency of 12.3 Hz were used to set the IMU sampling rate
(respecting the Nyquist theorem), while the acceleration range (±1.32 g) and angular velocity range (±4.91 rad/s) informed the sensor configuration.

## Key Results

- Compensates roll and pitch within ±30° on both axes
- Total weight: 133.4 g (spoon: 81 g)
- Overall length: 200 mm, comparable to a standard spoon
- Production cost: €69.28 (under the €100 target)
- Self-evaluation score: 34/40 across washability, safety, ergonomics and marketability criteria

## How to Use This Repository

- To review the firmware logic, see `firmware/TCS_control.ino`
- To inspect the PCB design, open the Eagle files (`.sch`/`.brd`) in `electronics/`
- To view the mechanical design, open the `.step` files inside `cad/.step Files/`
- To reproduce the tremor data analysis, open the MATLAB scripts in `data-analysis/`
