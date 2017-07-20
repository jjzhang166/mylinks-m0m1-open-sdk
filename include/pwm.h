/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#define CHA_DUTY		0xFFFFFE0F
#define CHB_DUTY		0xFFFFC1FF
#define CHA_TIME		0xFFFE3FFF
#define CHB_TIME		0xFFF1FFFF
#define CHA_POLAR		0xFFEFFFFF
#define CHB_POLAR		0xFFDFFFFF
#define PRE_SCALER		0xC03FFFFF

#define PWM1_A			0xFF8FFFFF
#define PWM1_B			0xFC7FFFFF
#define PWM2_A			0xE3FFFFFF
#define PWM2_B			0x1FFFFFFF

enum
{
	PWM_CHA_EN = 1,
	PWM_CHB_EN,
	PWM_EN,
	PWM_FREQ_ID_NUM = 31,
};

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
void pwm_set_freq(int pwm_ch, int id);
void pwm_set_duty(int pwm_ch, int duty);
void pwm_set_polarity(int pwm_ch, int value);
void pwm_set_enable(int pwm_ch, int value);
int pwm_get_freq(int pwm_ch);
int pwm_get_duty(int pwm_ch);
int pwm_get_polarity(int pwm_ch);

