// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_DEFINES_H
#define FA_DEFINES_H

#define GPIO_ONE_WIRE 5
#define GPIO_PWM1 15 // fresh air
#define GPIO_PWM2 4  // exhaust air
#define GPIO_PWM3 16 // frost
#define GPIO_SERVO 17
#define GPIO_LED 18

#define PWM_FREQ 25000
#define PWM1_CHANNEL 0
#define PWM2_CHANNEL 1
#define PWM3_CHANNEL 2
#define PWM_RESULTION 8

#define MAX_TEMP_SENSORS 2

#define I2C_SDA 21
#define I2C_SCL 22

#define FAN_CURVE_POINTS_MIN 2U
#define FAN_CURVE_POINTS_MAX 4U

#define FAN_LEVEL_MAX 10U
#define FAN_LEVEL_MIN 0U

#define FLAP_LEVEL_MAX 10U
#define FLAP_LEVEL_MIN 0U

#define FAN_LEVEL_STEPS 11U

#endif // FA_DEFINES_H
