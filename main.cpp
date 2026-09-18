#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

unsigned long lastTime;
unsigned long startTime;

float simulatedAngle = 0;

float setpoint = 0;
float kp = 6.0, ki = 1.0, kd = 2.0;
float error, lastError = 0, integral = 0, derivative;
float pidOutput;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  lastTime = millis();
  startTime = millis();
  Serial.println("time,angle,pid_output,disturbance");
}

void loop() {
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;
  if (dt <= 0) dt = 0.01;

  unsigned long elapsed = now - startTime;

  float disturbance = 0;
  if (elapsed > 2000 && elapsed < 5000) {
    disturbance = 15.0;
  }

  error = setpoint - simulatedAngle;
  integral += error * dt;
  integral = constrain(integral, -50, 50);
  derivative = (error - lastError) / dt;
  pidOutput = (kp * error) + (ki * integral) + (kd * derivative);
  lastError = error;
  pidOutput = constrain(pidOutput, -100, 100);

  simulatedAngle += (disturbance * dt * 2.0) + (pidOutput * dt * 0.15);

  Serial.print(elapsed); Serial.print(",");
  Serial.print(simulatedAngle); Serial.print(",");
  Serial.print(pidOutput); Serial.print(",");
  Serial.println(disturbance);

  delay(50);
}
