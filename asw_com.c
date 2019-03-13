/* 
 * File:   asw_com.c
 * Author: Cristian T. A.
 *
 * Created on August 28, 2018, 9:37 AM
 */

#include "general.h"
#include "rte.h"
#include "asw_com.h"
#include "asw_appLogic.h"
#include "asw_fsm.h"

/* nRF variables */
extern T_U8 au8RxBuff[];
extern T_U8 u8RxBuff_Index;
extern BOOL u8NewRX;

extern carInfo sCarsInfo[APP_MAX_NUM_CARS];
extern carInfo sMyInfo;
extern BOOL bIamFirst;

extern T_U8 u8NumSavedCars;
extern T_U8 u8FSM_currState;

/***********************************************************************************************************************
*  Function name    : COM_vCheckIRQ
*
*  Description      : Checks if there is a new received message OR if the transmission is complete.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vCheckIRQ()
{
    RTE_RF_vCheckIRQ();
}

/***********************************************************************************************************************
*  Function name    : COM_vSendMessage
*
*  Description      : Puts the RF module in the Transmit State, to transmit the message.
*                            
*  List of arguments: T_U8 u8Message - the message to be transmitted.
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vSendMessage(T_U8 u8Message)
{
    RTE_RF_vBeginTransmit(u8Message);
}

/***********************************************************************************************************************
*  Function name    : COM_vStartListening
*
*  Description      : Puts the RF module in the Receive State.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vStartListening()
{
    RTE_RF_vBeginReceive();
}

/****************************************/
/* Message decoding ********************/
/**************************************/

/***********************************************************************************************************************
*  Function name    : COM_vProcessMessage
*
*  Description      : Takes action depending on the message type and the current state 
*          of the Application's Finite State Machine (APP_FSM).
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vProcessMessage(T_U8 u8Message)
{
    if(COM_MSG__FIRST_ENTER == (CHECK_MSGTYPE_mask & u8Message))
    {
        if(APPSTATE_FOLLOW_LINE == u8FSM_currState)
        {
            bIamFirst = FALSE;
        }
        else if(APPSTATE_FIRST_ENTER == u8FSM_currState)
        {
            /* is it really possible ?! */
        }
        else if(APPSTATE_CROSS == u8FSM_currState || APPSTATE_FINISH == u8FSM_currState)
        {
            /* ignore the message. */
            return;
        }
        
        /* save car info */
        if(APP_MAX_NUM_CARS > u8NumSavedCars)
        {
            if(0x00 == (u8Message & CHECK_ROADNUM_mask))
            {
                sCarsInfo[u8NumSavedCars].u8RoadNum = 0;
            }
            else
            {
                sCarsInfo[u8NumSavedCars].u8RoadNum = (u8Message & CHECK_ROADNUM_mask) >> 2;
            }
            
            if(0x00 == (u8Message & CHECK_DIRECTION_mask))
            {
                sCarsInfo[u8NumSavedCars].u8Direction = 0;
            }
            else
            {
                sCarsInfo[u8NumSavedCars].u8Direction = (u8Message & CHECK_DIRECTION_mask) >> 4;
            }
            
            if(0x00 == (u8Message & CHECK_IS_ON_MAIN_mask))
            {
                sCarsInfo[u8NumSavedCars].bIsOnMain = FALSE;
            }
            else
            {
                sCarsInfo[u8NumSavedCars].bIsOnMain = TRUE;
            }
            
            ++u8NumSavedCars;
        }
        else
        {
            /* more cars than it can save. */
        }
    }
    else if(COM_MSG__EXIT == (CHECK_MSGTYPE_mask & u8Message))
    {
        T_U8 u8Index;
        u8Index = 0;
        
        if(0 == u8NumSavedCars)
        {
            /* a receptionat doar cand a iesit, nu si cand a intrat. */
            return;
        }
        
        T_U8 u8Road;
        if(0x00 == (CHECK_ROADNUM_mask & u8Message))
        {
            u8Road = 0;
        }
        else
        {
            u8Road = (CHECK_ROADNUM_mask & u8Message) >> 2;
        }                     
        
        while(u8Index < u8NumSavedCars)
        {
            
            if(sCarsInfo[u8Index].u8RoadNum == u8Road)
            {
                /* gasit masina care a terminat intersectia. */
                /* sterge-o din lista si rearanjeaza lista. */
                T_U8 i;
                for(i = u8Index; i < u8NumSavedCars - 1; ++i)
                {
                    sCarsInfo[i] = sCarsInfo[i + 1];
                }
                
                --u8NumSavedCars;
                
                return;
            }
            ++u8Index;
        }
        /* ajunge aici: nu a gasit masina care tocmai a iesit. */
    }
    else
    {
        /* alt mesaj => ERONAT */
    }
}

/***********************************************************************************************************************
*  Function name    : COM_vProcessFIFO
*
*  Description      : Processes every message in the message queue, if there is any available.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vProcessFIFO()
{
    if(TRUE == u8NewRX)
    {
        u8NewRX = FALSE;
        
        T_U8 i;
        for(i = 0; i < u8RxBuff_Index; ++i)
        {
            COM_vProcessMessage(au8RxBuff[i]);
        } 
        u8RxBuff_Index = 0;
    }
}

