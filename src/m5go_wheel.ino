/*
 * This file is part of the M5GO Balance Bot project
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 M5Stack (https://github.com/M5stack)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Encoder.h>
#include <L9110.h>

// ==================== I2C Slave =====================
uint8_t i2c_read_addr = 0;
#define SLAVE_ADDR             0x56
#define I2C_ADDR_OFFSET        0
#define MOTOR0_CTRL_ADDR       (I2C_ADDR_OFFSET + 0)
#define MOTOR0_CTRL_H_ADDR     (MOTOR0_CTRL_ADDR)
#define MOTOR0_CTRL_L_ADDR     (MOTOR0_CTRL_H_ADDR + 1)
#define MOTOR1_CTRL_ADDR       MOTOR0_CTRL_ADDR + 2
#define MOTOR1_CTRL_H_ADDR     (MOTOR1_CTRL_ADDR)
#define MOTOR1_CTRL_L_ADDR     (MOTOR1_CTRL_H_ADDR + 1)

// relative
#define ENCODER0_ADDR			MOTOR1_CTRL_ADDR + 2
#define ENCODER1_ADDR			ENCODER0_ADDR + 2

// absolute value
#define ENCODER0_ABS_ADDR		ENCODER1_ADDR + 2
#define ENCODER1_ABS_ADDR		ENCODER0_ABS_ADDR + 4

// ====================== Motor ========================
#define MOTOR0_A_PIN     6
#define MOTOR0_B_PIN     9
#define MOTOR1_A_PIN     3
#define MOTOR1_B_PIN     5

L9110 motor(MOTOR0_A_PIN, MOTOR0_B_PIN, MOTOR1_A_PIN, MOTOR1_B_PIN);

// ===================== Encoder =======================
#define ENCORE0_A_PIN    A2
#define ENCORE0_B_PIN    A3
#define ENCORE1_A_PIN    A0
#define ENCORE1_B_PIN    A1

Encoder encoder_0(ENCORE0_A_PIN, ENCORE0_B_PIN);
Encoder encoder_1(ENCORE1_A_PIN, ENCORE1_B_PIN);

int32_t encoder0_cnt = 0;
int32_t encoder1_cnt = 0;
int32_t encoder0_rel = 0;
int32_t encoder1_rel = 0;

// =================== I2c write event ==================
void receiveEvent(int howMany) {

	uint8_t request_addr = Wire.read();
	if (howMany == 1) {
		i2c_read_addr = request_addr;

	} else {
		if (request_addr == MOTOR0_CTRL_ADDR) {
			if (howMany == 5) {
				int16_t data[2];
				uint8_t* rxbuf = (uint8_t*)(&data);
				for (uint8_t i = 0; i < 4; i++) {
					rxbuf[i] = Wire.read();
				}
				motor.move(data[0], data[1]);
			}	
		}
	}
}

// =================== I2c read event ====================
void requestEvent()
{
	if (i2c_read_addr == ENCODER0_ABS_ADDR) {
		Wire.write(((uint8_t*)(&encoder0_cnt))[0]);
		Wire.write(((uint8_t*)(&encoder0_cnt))[1]);
		Wire.write(((uint8_t*)(&encoder0_cnt))[2]);
		Wire.write(((uint8_t*)(&encoder0_cnt))[3]);
		Wire.write(((uint8_t*)(&encoder1_cnt))[0]);
		Wire.write(((uint8_t*)(&encoder1_cnt))[1]);
		Wire.write(((uint8_t*)(&encoder1_cnt))[2]);
		Wire.write(((uint8_t*)(&encoder1_cnt))[3]);
	
	} else {
		int16_t relative_0 = encoder0_cnt - encoder0_rel;
		int16_t relative_1 = encoder1_cnt - encoder1_rel;
		Wire.write(((uint8_t*)(&relative_0))[0]);
		Wire.write(((uint8_t*)(&relative_0))[1]);
		Wire.write(((uint8_t*)(&relative_1))[0]);
		Wire.write(((uint8_t*)(&relative_1))[1]);
		encoder0_rel = encoder0_cnt;
		encoder1_rel = encoder1_cnt;
	}
}


void setup() {
	// put your setup code here, to run once:
	delay(200);
	motor.stop();

	Wire.begin(SLAVE_ADDR);
	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);
}

void loop() {
	encoder0_cnt = encoder_0.read();
	encoder1_cnt = encoder_1.read();
}
