/* Includes ------------------------------------------------------------------*/
#include "PWM_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    PWM_Handle;

/* Timer handler declaration */
TIM_HandleTypeDef    Encoder_Handle;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void TIMER_MSP_PWM_Init (void);
static void TIMER_MSP_ENCODER_Init (void);


void init_Timer_PWM (void)
{
	TIMER_MSP_PWM_Init();
	/*##-1- Configure the TIM peripheral #######################################*/ 
  /* Initialize TIMx peripheral as follow:
       + Prescaler = (SystemCoreClock/2)/18000000
       + Period = 1800  (to have an output frequency equal to 10 KHz)
       + ClockDivision = 0
       + Counter direction = Up
  */
  PWM_Handle.Instance = TIM_PWM;
  
  PWM_Handle.Init.Prescaler     = 42-1;
  PWM_Handle.Init.Period        = 1000-1;
  PWM_Handle.Init.ClockDivision = 0;
  PWM_Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(&PWM_Handle) != HAL_OK)
  {
    /* Initialization Error */
  }
  
  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sConfig.OCMode     = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 1 */
  sConfig.Pulse = 0;  
  if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
  }
}

void init_Timer_ENCODER(void) 
{
	TIMER_MSP_ENCODER_Init();
	
	/* -1- Initialize TIM1 to handle the encoder sensor */
  /* Initialize TIM1 peripheral as follow:
       + Period = 65535
       + Prescaler = 0
       + ClockDivision = 0
       + Counter direction = Up
  */
  Encoder_Handle.Instance = TIM_ENCODER;  
  
  Encoder_Handle.Init.Period            = 65535;
  Encoder_Handle.Init.Prescaler         = 0;
  Encoder_Handle.Init.ClockDivision     = 0;
  Encoder_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Encoder_Handle.Init.RepetitionCounter = 0; 
    
  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;
  
  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC1Filter          = 0;
  
  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC2Filter          = 0; 
  
  if(HAL_TIM_Encoder_Init(&Encoder_Handle, &sEncoderConfig) != HAL_OK)
  {
    /* Initialization Error */
  }
  TIM_ENCODER->CNT = 0;
  /* Start the encoder interface */
  HAL_TIM_Encoder_Start(&Encoder_Handle, TIM_CHANNEL_ALL);

}

static void TIMER_MSP_PWM_Init (void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIM_PWM_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  TIM_PWM_CLK_CHANNEL_GPIO_PORT();
  PWM_DIR_CLK_CHANNEL_GPIO_PORT();
	PWM_EN_CLK_CHANNEL_GPIO_PORT();
  /*##-2- Configure I/Os #####################################################*/
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  
  GPIO_InitStruct.Pin = TIM_PWM_GPIO_PIN_CHANNEL1;
  HAL_GPIO_Init(TIM_PWM_GPIO_PORT, &GPIO_InitStruct);
	
	/* DIR, EN pins configuration */
	GPIO_InitStruct.Pin = PWM_GPIO_PIN_DIR;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(PWM_DIR_GPIO_PORT, &GPIO_InitStruct); 
	
	GPIO_InitStruct.Pin = PWM_GPIO_PIN_EN;
	HAL_GPIO_Init(PWM_EN_GPIO_PORT, &GPIO_InitStruct);
  
}

static void TIMER_MSP_ENCODER_Init (void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIM1 Peripheral clock enable */
  TIM_ENCODER_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  TIM_ENCODER_CHANNEL_1_GPIO_PORT();
  TIM_ENCODER_CHANNEL_2_GPIO_PORT();
  
  /*##-2- Configure I/Os #####################################################*/  
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = TIM_ENCODER_GPIO_AF_TIM;
  
  /* Channel 1 configuration */
  GPIO_InitStruct.Pin = TIM_ENCODER_GPIO_PIN_CHANNEL_1;
  HAL_GPIO_Init(TIM_ENCODER_GPIO_PORT_1, &GPIO_InitStruct);
  
  /* Channel 2 configuration */
  GPIO_InitStruct.Pin = TIM_ENCODER_GPIO_PIN_CHANNEL_2;
  HAL_GPIO_Init(TIM_ENCODER_GPIO_PORT_2, &GPIO_InitStruct);
}


int32_t get_current_posistion(void)
{
	static int32_t 	p = 0, p_pre = 0, dp = 0;
  static int32_t 	pulse_cur = 0.0;
	
	p = (int32_t)TIM_ENCODER->CNT;
	dp = p - p_pre;
	if (dp > 32768)
	{
		dp -= 65536;
	}
	else if (dp < -32768)
	{
		dp += 65536;
	}
	p_pre = p;
	pulse_cur += dp;
	
	return pulse_cur; 
}

/* Output pwm frequency - unit: 0.1% */
void PWM0_Set_Duty(int16_t d)
{
	if (d<-1000)
		d = -1000;
	else if (d>1000)
		d = 1000;

	if (d == 0){
		HAL_GPIO_WritePin(PWM_EN_GPIO_PORT, PWM_GPIO_PIN_EN, GPIO_PIN_RESET);  	// disable pwm
	}	
	else if (d > 0)	{
		HAL_GPIO_WritePin(PWM_EN_GPIO_PORT, PWM_GPIO_PIN_EN, GPIO_PIN_SET);	// enabe pwm
		HAL_GPIO_WritePin(PWM_DIR_GPIO_PORT, PWM_GPIO_PIN_DIR, GPIO_PIN_RESET); 	// dir = 1
	}
	else 	{
		HAL_GPIO_WritePin(PWM_EN_GPIO_PORT, PWM_GPIO_PIN_EN, GPIO_PIN_SET);	// enabe pwm
		HAL_GPIO_WritePin(PWM_DIR_GPIO_PORT, PWM_GPIO_PIN_DIR, GPIO_PIN_SET); 	// dir = 0
		d = 1000 + d;
	}
	d = (TIM_PWM->ARR+1)*d/1000;
	
	/* Set the pulse value for channel 1 */
	sConfig.Pulse = d;  
	if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		/* Configuration Error */
	}
			 /* Start channel 1 */
	if(HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		/* Starting Error */
	}
}

/* Setup pwm frequency - unit: Hz */
void PWM0_Set_Freq(uint32_t freq)
{
	uint32_t period;
	
	period = 84000000/(freq*(TIM_PWM->PSC+1))-1;	
	if (period > 0xffff)
		period = 0xffff;
	
	TIM_PWM->ARR = period;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
