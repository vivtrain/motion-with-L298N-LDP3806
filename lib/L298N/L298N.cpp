#include <Arduino.h>
#include "L298N.h"

/// @brief Constructs a L298N object for bidirectinoal brushed DC motor control.
/// @param pin1 PWM pin that controls forward direction.
/// @param pin2 PWM pin that controls reverse direction.
L298N::L298N(uint8_t pin1, uint8_t pin2) {
    pwm_forward_pin_ = pin1;
    pwm_reverse_pin_ = pin2;
}

/// @brief Apply voltage on a duty cycle to the output in the forward or reverse direction.
/// @param output PWM duty cycle expressed on the range [-255, 255]. Positive values use the
///     foward pin. Negative values use the reverse pin.
void L298N::set(int output) {
    // Check for valid input
    if (output < -255 || output > 255)
        return;
    
    unsigned int pwmOutput = abs(output);
    if (output > 0) {
        analogWrite(pwm_forward_pin_, pwmOutput);
        analogWrite(pwm_reverse_pin_, 0);
    } else if (output < 0) {
        analogWrite(pwm_forward_pin_, 0);
        analogWrite(pwm_reverse_pin_, pwmOutput);
    } else {
        analogWrite(pwm_forward_pin_, 0);
        analogWrite(pwm_reverse_pin_, 0);
    }
}

