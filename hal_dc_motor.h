/* 
 * File:   hal_dc_motor.h
 * Author: uia95516
 *
 * Created on August 23, 2018, 10:12 AM
 */

#ifndef HAL_DC_MOTOR_H
#define	HAL_DC_MOTOR_H

#include "general.h"


#define  ACCELER    0.1
#define  DECELER    2
#define  FORWARD    0
#define  BACKWARD   1


void DCMOT_vInit(void);
void DCMOT_vSetDirSpeed( T_U8 u8Dir, T_F16 u8Speed);




#endif	/* HAL_DC_MOTOR_H */

