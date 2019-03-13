/* 
 * File:   asw_com.h
 * Author: Cristian T. A.
 *
 * Created on August 28, 2018, 9:38 AM
 */

#ifndef ASW_COM_H
#define	ASW_COM_H

#define ASW_COM_DIM_FIFO 64

/**************** Message Types ************************************************/
/* The road that I'm on, is the main one */ 
#define COM_MSG__FIRST_ENTER    0b00000001
/* I finished the curve. Delete me from the Array */
#define COM_MSG__EXIT           0b00000011

/**************** Message Masks ************************************************/
#define CHECK_IS_ON_MAIN_mask   0b01000000
#define CHECK_DIRECTION_mask    0b00110000
#define CHECK_ROADNUM_mask      0b00001100
#define CHECK_MSGTYPE_mask      0b00000011

/*******************************************************************************/

void COM_vCheckIRQ(void);
void COM_vSendMessage(T_U8 u8Message);
void COM_vStartListening(void);
void COM_vProcessMessage(T_U8 u8Message);
void COM_vProcessFIFO(void);


#endif	/* ASW_COM_H */

