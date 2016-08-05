/*
 * testPWM.h
 *
 * Created: 04.08.2016 17:41:33
 *  Author: UB4LAG Vasily Afanasyev
 */ 


#ifndef TESTPWM_H_
#define TESTPWM_H_

#define PWM_PERIOD 30000U /* 15mS */
#define PWM_MIN 1600U /* 0.8mS */
#define PWM_MAX 4400U /* 2.2mS */

#define PinPWMa B,1,H
#define PinPWMb B,2,H
#define PinLed B,5,H

void initial_periphery(void);

#endif /* TESTPWM_H_ */