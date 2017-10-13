/*
 * EGG OpenSource EBike firmware
 *
 * Copyright (C) Casainho, 2015, 2106, 2017.
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include <stdio.h>
#include "motor.h"

uint16_t target_erps = 0;

void motor_speed_controller_set_erps (uint16_t erps)
{
  target_erps = erps;
}

// call every 100ms
void motor_speed_controller (void)
{
  int16_t i16_error;
  int16_t i16_p_term;
  static int16_t i16_i_term;
  int16_t i16_output;

  i16_error = target_erps - motor_get_motor_speed_erps ();
  i16_p_term = i16_error * MOTOR_SPEED_CONTROLLER_KP;
  i16_i_term += i16_error * MOTOR_SPEED_CONTROLLER_KI;
  // windup control
  if (i16_i_term > MOTOR_SPEED_CONTROLLER_OUTPUT_MAX) i16_i_term = MOTOR_SPEED_CONTROLLER_OUTPUT_MAX;
  else if (i16_i_term < 1) i16_i_term = 0;

  i16_output = i16_p_term + i16_i_term;
  // limit max output value
  if (i16_output > MOTOR_SPEED_CONTROLLER_OUTPUT_MAX) i16_output = MOTOR_SPEED_CONTROLLER_OUTPUT_MAX;
  else if (i16_output < 1) i16_output = 0;
  i16_output >>= 4; // divide to 16, as MOTOR_SPEED_CONTROLLER_KP and MOTOR_SPEED_CONTROLLER_KI are 16x; avoid using floats

//  motor_set_pwm_duty_cycle_target ((uint8_t) i16_output);

  printf("%d, %d, %d, %d\n", i16_error, i16_p_term, i16_i_term, i16_output);
}
