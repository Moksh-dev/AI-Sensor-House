
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

const int GAS_PIN = 34;
const float MOTION_THRESHOLD = 2.0;

float previousMagnitude = 9.81;
bool firstReading = true;

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);
    pinMode(GAS_PIN, INPUT);

    if (!mpu.begin()) {
        Serial.println("MPU6050 not detected!");
        while (true) {
            delay(10);
        }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Both sensors initialized");
}

void loop() {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    float x = accel.acceleration.x;
    float y = accel.acceleration.y;
    float z = accel.acceleration.z;

    float magnitude = sqrt(x * x + y * y + z * z);

    if (firstReading) {
        previousMagnitude = magnitude;
        firstReading = false;
    }

    float change = fabs(magnitude - previousMagnitude);
    previousMagnitude = magnitude;

    bool motion = change > MOTION_THRESHOLD;
    int gasLevel = analogRead(GAS_PIN);

    Serial.print("{\"gas_level\":");
    Serial.print(gasLevel);
    Serial.print(",\"accel_magnitude\":");
    Serial.print(magnitude, 2);
    Serial.print(",\"accel_change\":");
    Serial.print(change, 2);
    Serial.print(",\"motion\":");
    Serial.print(motion ? "true" : "false");
    Serial.println("}");

    delay(500);
}
