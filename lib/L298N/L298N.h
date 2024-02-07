#ifndef L298N_H
#define L298N_H

#include <Arduino.h>

class L298N {
  public:
    L298N(uint8_t pin1, uint8_t pin2);
    void set(int output);
  private:
    uint8_t pwm_forward_pin_;
    uint8_t pwm_reverse_pin_;
};

#endif