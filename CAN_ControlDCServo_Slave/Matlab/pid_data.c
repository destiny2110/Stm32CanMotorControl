/*
 * File: pid_data.c
 *
 * Real-Time Workshop code generated for Simulink model pid.
 *
 * Model version                        : 1.23
 * Real-Time Workshop file version      : 7.4  (R2009b)  29-Jun-2009
 * Real-Time Workshop file generated on : Sat May 13 18:40:35 2017
 * TLC version                          : 7.4 (Jul 14 2009)
 * C/C++ source code generated on       : Sat May 13 18:40:36 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pid.h"
#include "pid_private.h"

/* Block parameters (auto storage) */
Parameters_pid pid_P = {
  360.0,                              /* Expression: 1800
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  800.0,                               /* Computed Parameter: Setpointdeg_Period
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  400.0,                               /* Computed Parameter: Setpointdeg_Duty
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  0.07,                               /* Expression: P
                                        * Referenced by: '<S1>/Proportional Gain'
                                        */
  0.005,                               /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S1>/Integrator'
                                        */
  0.0,                                 /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S1>/Integrator'
                                        */
  0.007,                                 /* Expression: D
                                        * Referenced by: '<S1>/Derivative Gain'
                                        */
  0.005,                               /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S1>/Filter'
                                        */
  0.0,                                 /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S1>/Filter'
                                        */
  100.0,                               /* Expression: N
                                        * Referenced by: '<S1>/Filter Coefficient'
                                        */
  12.0,                                /* Expression: 12
                                        * Referenced by: '<Root>/Saturation'
                                        */
  -12.0,                               /* Expression: -12
                                        * Referenced by: '<Root>/Saturation'
                                        */
  0.001,                              /* Expression: I
                                        * Referenced by: '<S1>/Integral Gain'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  400.0,                               /* Computed Parameter: PulseGenerator_Period
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  200.0,                               /* Computed Parameter: PulseGenerator_Duty
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  0.0                                  /* Expression: 0
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
};

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
