# Tremor Control Spoon

An electronic Tremor Control Spoon designed to compensate involuntary hand 
tremors in Parkinson's disease patients during meals, developed as part of the 
Biodesign course at Università Campus Bio-Medico di Roma in 2023 by a team of 10 students.

---

## Overview

This project focused on the design and development of an electronically 
stabilized spoon to assist individuals with Parkinson's disease by compensating 
tremors during meal consumption, promoting greater independence and comfort in 
daily life. The work was developed in collaboration with a team of colleagues, 
with my contribution — focused on mechanical design and the control system — 
made remotely while based in Vienna.

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
  - `TCS_control.ino`, `Functions.c`, `Variables.h`
- `electronics/` — PCB schematic, board layout, and bill of materials (Eagle CAD)
- `cad/` — Mechanical design files (Solid Edge/STEP) for the handle, motor case, 
  and spoon tool
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

## Key Results

- Compensates roll and pitch within ±30° on both axes
- Total weight: 133.4 g (spoon: 81 g)
- Overall length: 200 mm, comparable to a standard spoon
- Production cost: €69.28 (under the €100 target)
- Self-evaluation score: 34/40 across washability, safety, ergonomics, and 
  marketability criteria

## How to Use This Repository

- To review the firmware logic, see `firmware/TCS_control.ino`
- To inspect the PCB design, open the Eagle files in `electronics/`
- To view the mechanical design, open the `.step` files in `cad/` with any CAD 
  software (e.g., Solid Edge, FreeCAD)
- To reproduce the tremor data analysis, open the MATLAB scripts in 
  `data-analysis/`
- For the full methodology, results, and discussion, see `docs/`


