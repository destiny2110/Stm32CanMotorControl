/*
 * File: pid.h
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

#ifndef RTW_HEADER_pid_h_
#define RTW_HEADER_pid_h_
#ifndef pid_COMMON_INCLUDES_
# define pid_COMMON_INCLUDES_
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#include "rt_SATURATE.h"
#endif                                 /* pid_COMMON_INCLUDES_ */

#include "pid_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S1>/Integrator' */
  real_T Filter_DSTATE;                /* '<S1>/Filter' */
  int32_T clockTickCounter;            /* '<Root>/Setpoint (deg)' */
  int32_T clockTickCounter_n;          /* '<Root>/Pulse Generator' */
} D_Work_pid;

/* Parameters (auto storage) */
struct Parameters_pid_ {
  real_T Setpointdeg_Amp;              /* Expression: 1800
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  real_T Setpointdeg_Period;           /* Computed Parameter: Setpointdeg_Period
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  real_T Setpointdeg_Duty;             /* Computed Parameter: Setpointdeg_Duty
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  real_T Setpointdeg_PhaseDelay;       /* Expression: 0
                                        * Referenced by: '<Root>/Setpoint (deg)'
                                        */
  real_T ProportionalGain_Gain;        /* Expression: P
                                        * Referenced by: '<S1>/Proportional Gain'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S1>/Integrator'
                                        */
  real_T Integrator_IC;                /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S1>/Integrator'
                                        */
  real_T DerivativeGain_Gain;          /* Expression: D
                                        * Referenced by: '<S1>/Derivative Gain'
                                        */
  real_T Filter_gainval;               /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S1>/Filter'
                                        */
  real_T Filter_IC;                    /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S1>/Filter'
                                        */
  real_T FilterCoefficient_Gain;       /* Expression: N
                                        * Referenced by: '<S1>/Filter Coefficient'
                                        */
  real_T Saturation_UpperSat;          /* Expression: 12
                                        * Referenced by: '<Root>/Saturation'
                                        */
  real_T Saturation_LowerSat;          /* Expression: -12
                                        * Referenced by: '<Root>/Saturation'
                                        */
  real_T IntegralGain_Gain;            /* Expression: I
                                        * Referenced by: '<S1>/Integral Gain'
                                        */
  real_T PulseGenerator_Amp;           /* Expression: 1
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  real_T PulseGenerator_Period;        /* Computed Parameter: PulseGenerator_Period
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  real_T PulseGenerator_Duty;          /* Computed Parameter: PulseGenerator_Duty
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
  real_T PulseGenerator_PhaseDelay;    /* Expression: 0
                                        * Referenced by: '<Root>/Pulse Generator'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_pid {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_pid pid_P;

/* Block states (auto storage) */
extern D_Work_pid pid_DWork;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  RTW declares the memory for these signals
 * and exports their symbols.
 *
 */
extern real_T In2;                     /* '<Root>/In2' */
extern real_T str_setpont;             /* '<Root>/Setpoint (deg)' */
extern real_T Out1;                    /* '<Root>/Saturation' */
extern real_T Out2;                    /* '<Root>/Pulse Generator' */

/* Model entry point functions */
extern void pid_initialize(void);
extern void pid_step(void);
extern void pid_terminate(void);

/* Real-time Model object */
extern RT_MODEL_pid *pid_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : pid
 * '<S1>'   : pid/Discrete PID  Controller
 */
#endif                                 /* RTW_HEADER_pid_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
