#pragma once

#include <inttypes.h>

class L9110 {
public:
    L9110(uint8_t A_1A, uint8_t A_1B, uint8_t B_1A, uint8_t B_1B);

    void move(int speedA, int speedB);
    void stop();

private:
    uint8_t MOTOR_A_1A;
    uint8_t MOTOR_A_1B;
    uint8_t MOTOR_B_1A;
    uint8_t MOTOR_B_1B;
};
