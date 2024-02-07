#include <Arduino.h>
#include <RotaryEncoder.h>
#include <L298N.h>
#include "util.h"

RotaryEncoder encoder(2, 3, RotaryEncoder::LatchMode::TWO03);
L298N motorController(9, 10);

const int kTicksPerRev = 1200;
const int kMaxOutput = 255;

const double kMaxAngVelocity = 1.3; // revs/second
const double kVelocityFF = 1/kMaxAngVelocity * kMaxOutput;
const double kProportional = 1.0 * kVelocityFF;
double setpoint = 1.0;

const int maxTime = 5e3;
unsigned long counter = 0;
unsigned long prevTime, curTime;
int prevPos = 0;

void setup() {
    // Needs fast baud rate, ensure platformio.ini has monitor_speed to match
    Serial.begin(115200);
    while (!Serial)
        continue;
    Serial.println("Polling Rotary Encoder and H-bridge motor controller");
    Serial.println("time, position, error, angular velocity, PWM output");
    prevTime = millis();
}

void loop() {
    // Poll the encoder
    encoder.tick();

    // Every five hundred loops (approx 20ms)
    if (counter % 500 == 0) {
        // Measure change in time
        curTime = millis();
        unsigned long dT = curTime - prevTime;

        int position = encoder.getPosition(); // in ticks
        // ticks/millis * millis/second * revs/ticks = revs/second
        double angularVelocity = double(position - prevPos) / dT * 1000./kTicksPerRev; // in revs/second
  
        setpoint = (curTime/1000+2) * 0.2;

        // Calculate error 
        double error = setpoint - angularVelocity;

        // Calculate control output
        int pwmOutput =
            kVelocityFF * setpoint +
            kProportional * error;
        pwmOutput = constrain(pwmOutput, -kMaxOutput, kMaxOutput);

        // Log every 500 loops for the first 5 seconds
        if (counter % 500 == 0 && curTime < maxTime) {
            Serial.print(curTime/1000.);
            Serial.print(", ");
            Serial.print(setpoint);
            Serial.print(", ");
            Serial.print(angularVelocity);
            Serial.print(", ");
            Serial.print(error);
            Serial.print(", ");
            Serial.println(pwmOutput);
        }

        // Apply motor control
        if (curTime < maxTime) {
            motorController.set(pwmOutput);
        } else {
            motorController.set(0);
        }
        prevTime = curTime;
        prevPos = position;
    }

    // Set up for next loop
    counter++;
}