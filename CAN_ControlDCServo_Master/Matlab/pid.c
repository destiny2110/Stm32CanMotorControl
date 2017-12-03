/*
 * File: pid.c
 *
 * Real-Time Workshop code generated for Simulink model pid.
 *
 * Model version                        : 1.22
 * Real-Time Workshop file version      : 7.4  (R2009b)  29-Jun-2009
 * Real-Time Workshop file generated on : Sun Apr 09 07:39:25 2017
 * TLC version                          : 7.4 (Jul 14 2009)
 * C/C++ source code generated on       : Sun Apr 09 07:39:26 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pid.h"
#include "pid_private.h"

/* Exported block signals */
real_T In2;                            /* '<Root>/In2' */
real_T Out1;                           /* '<Root>/Saturation' */
real_T Out2;                           /* '<Root>/Pulse Generator' */

/* Block states (auto storage) */
D_Work_pid pid_DWork;

/* Real-time model */
RT_MODEL_pid pid_M_;
RT_MODEL_pid *pid_M = &pid_M_;

/* Model step function */
void pid_step(void)
{
  real_T rtb_Add;
  real_T rtb_Sum;
  real_T rtb_FilterCoefficient;

  /* DiscretePulseGenerator: '<Root>/Setpoint (deg)' */
  rtb_Add = ((real_T)pid_DWork.clockTickCounter < pid_P.Setpointdeg_Duty) &&
    (pid_DWork.clockTickCounter >= 0) ? pid_P.Setpointdeg_Amp : 0.0;
  if ((real_T)pid_DWork.clockTickCounter >= pid_P.Setpointdeg_Period - 1.0) {
    pid_DWork.clockTickCounter = 0;
  } else {
    pid_DWork.clockTickCounter = pid_DWork.clockTickCounter + 1;
  }

  /* Sum: '<Root>/Add' incorporates:
   *  Inport: '<Root>/In2'
   */
  rtb_Add -= In2;

  /* Gain: '<S1>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S1>/Filter'
   *  Gain: '<S1>/Derivative Gain'
   *  Sum: '<S1>/SumD'
   */
  rtb_FilterCoefficient = (pid_P.DerivativeGain_Gain * rtb_Add -
    pid_DWork.Filter_DSTATE) * pid_P.FilterCoefficient_Gain;

  /* Sum: '<S1>/Sum' incorporates:
   *  DiscreteIntegrator: '<S1>/Integrator'
   *  Gain: '<S1>/Proportional Gain'
   */
  rtb_Sum = (pid_P.ProportionalGain_Gain * rtb_Add + pid_DWork.Integrator_DSTATE)
    + rtb_FilterCoefficient;

  /* Saturate: '<Root>/Saturation' */
  Out1 = rt_SATURATE(rtb_Sum, pid_P.Saturation_LowerSat,
                     pid_P.Saturation_UpperSat);

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  Out2 = ((real_T)pid_DWork.clockTickCounter_n < pid_P.PulseGenerator_Duty) &&
    (pid_DWork.clockTickCounter_n >= 0) ? pid_P.PulseGenerator_Amp : 0.0;
  if ((real_T)pid_DWork.clockTickCounter_n >= pid_P.PulseGenerator_Period - 1.0)
  {
    pid_DWork.clockTickCounter_n = 0;
  } else {
    pid_DWork.clockTickCounter_n = pid_DWork.clockTickCounter_n + 1;
  }

  /* Update for DiscreteIntegrator: '<S1>/Integrator' incorporates:
   *  Gain: '<S1>/Integral Gain'
   */
  pid_DWork.Integrator_DSTATE = pid_P.IntegralGain_Gain * rtb_Add *
    pid_P.Integrator_gainval + pid_DWork.Integrator_DSTATE;

  /* Update for DiscreteIntegrator: '<S1>/Filter' */
  pid_DWork.Filter_DSTATE = pid_P.Filter_gainval * rtb_FilterCoefficient +
    pid_DWork.Filter_DSTATE;
}

/* Model initialize function */
void pid_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(pid_M, (NULL));

  /* block I/O */

  /* exported global signals */
  Out1 = 0.0;
  Out2 = 0.0;

  /* states (dwork) */
  (void) memset((void *)&pid_DWork, 0,
                sizeof(D_Work_pid));

  /* external inputs */
  In2 = 0.0;

  /* Start for DiscretePulseGenerator: '<Root>/Setpoint (deg)' */
  pid_DWork.clockTickCounter = 0;

  /* Start for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  pid_DWork.clockTickCounter_n = 0;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Integrator' */
  pid_DWork.Integrator_DSTATE = pid_P.Integrator_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Filter' */
  pid_DWork.Filter_DSTATE = pid_P.Filter_IC;
}

/* Model terminate function */
void pid_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
