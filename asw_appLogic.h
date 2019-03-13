/* 
 * File:   asw_appLogic.h
 * Author: Cristian T. A.
 *
 * Created on September 18, 2018, 2:18 PM
 */

#ifndef ASW_APPLOGIC_H
#define	ASW_APPLOGIC_H

#define DEBUG_DIR DIR_FWD

/* Car's speed in different states */
#define APP_SPEED__LINE         40
#define APP_SPEED__BEFORESTOP   20
#define APP_SPEED__CROSS        25
/* 45 20 27 */

/* Encoder ticks needed to exit intersection in different directions */
#define ENC_PATH_LEN_FWD    7450;
#define ENC_PATH_LEN_LEFT   7450;
#define ENC_PATH_LEN_RIGHT  3700;

#define DIR_UNDEF 0
#define DIR_RIGHT 1
#define DIR_LEFT  2
#define DIR_FWD   3

#define APP_MAX_NUM_CARS 4

T_U8 getOpositeRoad(T_U8);
T_U8 getRandomDir();

#endif	/* ASW_APPLOGIC_H */

