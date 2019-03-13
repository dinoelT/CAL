/* 
 * File:   hal_line_follower.h
 * Author: Nicolae Hagiu
 *
 * Created on August 20, 2028, 8:14 AM
 */


#ifndef HAL_LINEFOLLOWER_H
#define	HAL_LINEFOLLOWER_H

#include "general.h"


#define ANGLE_FRONT 90
#define ANGLE_LEFT  60
#define ANGLE_RIGHT 120

#define NR_IRLED 6


void vSetDirLineFollower(BOOL u16SetIntOut);
void vDoWriteLineFollower(void);
T_U8 u8GetValueLineFollower(void);

void vDoLineFollower(void);

#endif	/* HAL_LINEFOLLOWER_H */

