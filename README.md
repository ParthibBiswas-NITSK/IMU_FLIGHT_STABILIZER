# IMU-Based Flight Stabilisation Controller

A simulated flight-control system built on ESP32, demonstrating the sensor fusion and PID control loop used in drone flight controllers. Built and tested in Wokwi.

**Stack:** ESP32, MPU6050 (IMU), C++, PID Control

---

## What it does

Drones stay level by constantly sensing their tilt and correcting it. This project simulates that loop:

1. Reads orientation data from an MPU6050 IMU
2. Runs a PID controller to correct any tilt back to level (0°)
3. Injects a simulated 15° disturbance (like a wind gust) to test recovery
4. Logs the response as time-series data

---

## How the control loop works

Every 50ms:
- **Sense** current angle
- **Compare** it to the target (0° = level) → `error`
- **Calculate** a correction using PID (Proportional + Integral + Derivative)
- **Apply** the correction to pull the angle back to level

This is the same core logic used in real flight controllers (Betaflight, ArduPilot, PX4).

---

## Results

A 15° disturbance was injected between t=2s–5s.

![Stabilization Graph](time,%20angle,%20pid_output%20and%20disturbance.png)

- **0–2s:** flat, stable at 0°
- **2–5s:** disturbance hits, angle rises to ~47.6°, PID output saturates fighting back
- **5–12s:** disturbance removed, angle recovers, small overshoot to -6°, then settles

Full data: [`stabilization_data.csv`](./stabilization_data.csv)

---

## Key debugging fixes

- Fixed a **sign error** that caused the correction to worsen the tilt instead of fixing it
- Added **anti-windup clamping** to stop the integral term from getting stuck at max output

---
