#include <Arduino.h>

/// @brief Gets the sign of an integer.
/// @param num The number to test.
/// @return -1 for negative numbers, 1 for positive numbers, 0 for zero.
uint8_t signOf(int num) {
    return (num < 0 ? -1 : (num > 0 ? 1 : 0));
}