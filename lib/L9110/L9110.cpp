#include "L9110.h"
#include "Arduino.h"

L9110::L9110(uint8_t A_1A, uint8_t A_1B, uint8_t B_1A, uint8_t B_1B)
    : MOTOR_A_1A(A_1A)
    , MOTOR_A_1B(A_1B)
    , MOTOR_B_1A(B_1A)
    , MOTOR_B_1B(B_1B)
{
    pinMode(MOTOR_A_1A, OUTPUT);
    pinMode(MOTOR_A_1B, OUTPUT);
    pinMode(MOTOR_B_1A, OUTPUT);
    pinMode(MOTOR_B_1B, OUTPUT);
    stop();
}

void L9110::move(int speedA, int speedB)
{
    analogWrite(MOTOR_A_1A, speedA > 0 ? 0 : abs(speedA));
    analogWrite(MOTOR_A_1B, speedA < 0 ? 0 : abs(speedA));

    analogWrite(MOTOR_B_1A, speedB < 0 ? 0 : abs(speedB));
    analogWrite(MOTOR_B_1B, speedB > 0 ? 0 : abs(speedB));
}

void L9110::stop()
{
    analogWrite(MOTOR_A_1A, LOW);
    analogWrite(MOTOR_A_1B, LOW);
    analogWrite(MOTOR_B_1A, LOW);
    analogWrite(MOTOR_B_1B, LOW);
}
