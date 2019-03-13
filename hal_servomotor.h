/* 
 * File:   hal_servomotor.h
 * Author: Nicolae Hagiu
 *
 * Created on August 17, 2028, 1:27 PM
 */

#ifndef HAL_SERVOMOTOR_H
#define	HAL_SERVOMOTOR_H

#include "general.h"

void vSetServoAngle(T_F16 angle);

#define  ANGLE_MIN 0
#define  ANGLE_MAX 180
#define  LIMIT_INF 60
#define  LIMIT_SUP 120
#define  OFFSET 4
/* The offset is the Lower bound of the duty cycle interval*/
#define  GAIN   0.117
/********** How to calculate gain **************/
/* The period is 20mS                          */
/* Left -> 4% duty cycle, equivalent to 0.8mS  */
/* Right-> 11% duty cycle, equivalent to 2.2mS */
/* Length of percent interval = 11 - 4 = 7%    */
/***********************************************/
/* Left Angle -> 60 degrees                    */
/* Right Angle-> 120 degrees                   */
/* Length of angle interval = 120 - 60 = 60    */
/* Resolution (GAIN) = 7 / 60 = 0.117          */

#endif	/* HAL_SERVOMOTOR_H */

