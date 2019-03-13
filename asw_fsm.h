/* 
 * File:   asw_fsm.h
 * Author: Cristian T. A.
 *
 * Created on August 30, 2018, 2:45 PM
 */

#ifndef ASW_FSM_H
#define	ASW_FSM_H

/* Application FSM states */
#define APPSTATE_OBST_DETECT    0
#define APPSTATE_FOLLOW_LINE    1
#define APPSTATE_FIRST_ENTER    2
#define APPSTATE_BEFORE_STOP    3
#define APPSTATE_STOP_LINE      4
#define APPSTATE_CROSS          5
#define APPSTATE_FINISH         6


void vObstLogic(void);
void APP_FSM(void);

#endif	/* ASW_FSM_H */

