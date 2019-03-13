/* 
 * File:   asw_fsm.c
 * Author: Cristian T. A.
 *
 * Created on August 30, 2018, 2:45 PM
 */

#include "hal_lineFollower.h"
#include "hal_dc_motor.h"
#include "hal_servomotor.h"
#include "asw_com.h"
#include "mcal_encoder.h"
#include "asw_appLogic.h"
#include "asw_fsm.h"


extern BOOL bStopLine;
extern BOOL bObstDetected;
extern carInfo sCarsInfo[APP_MAX_NUM_CARS];
extern carInfo sMyInfo;
extern T_U8 u8NumSavedCars;
extern BOOL bIamFirst;

extern T_U16 u16EncoderPathLength;
extern T_U16 u16EncoderPassed;
extern BOOL bEncoderSet;
extern BOOL bWhite;

/* u8FSMstate can be one of the above states. */
/* current state */
T_U8 u8FSM_currState = APPSTATE_FOLLOW_LINE;
/* preceding state */
T_U8 u8FSM_precState = APPSTATE_FOLLOW_LINE;


/***********************************************************************************************************************
*  Function name    : vObstLogic
*
*  Description      : Checks if the Obstacle Sensor detected any obstacle. If true, it stops. If false, 
*              it continues from where it last left off.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void vObstLogic()
{
    if(FALSE == bObstDetected)
    {
        /* if the obstacle was here but now is gone: */
        if(APPSTATE_OBST_DETECT == u8FSM_currState)
        {
            /* update states */
            /* get back to the state before OBST_DETECT. */
            u8FSM_currState = u8FSM_precState;
            u8FSM_precState = APPSTATE_OBST_DETECT;
        }
        /* if the obstacle wasn't here before and is not here now: */
        else
        {
            /* nothing special. let the program flow. */
        }
    }
    else
    {
        /* if the obstacle was here and is still here now: */
        if(APPSTATE_OBST_DETECT == u8FSM_currState)
        {
            /* don't update states. Remain in OBST_DETECT state. */
        }
        /* if this is the first time the object is here: */
        else
        {
            /* update states */
            u8FSM_precState = u8FSM_currState;
            u8FSM_currState = APPSTATE_OBST_DETECT;
        }
    }
}


/***********************************************************************************************************************
*  Function name    : APP_FSM
*
*  Description      : The Finite State Machine of the Application.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void APP_FSM()
{    
    /* Obstacle Logic has the highest priority. It executes before any other state. */
    vObstLogic();

    switch(u8FSM_currState)
    {                    /* Application States */
/**********************************************/          
/******/case APPSTATE_FOLLOW_LINE:/***********/
/********************************************/            
            DCMOT_vSetDirSpeed(FORWARD, APP_SPEED__LINE);
            
            if(TRUE == bStopLine)
            {
                bStopLine = FALSE;
                
                /* Update state */
                u8FSM_precState = u8FSM_currState;
                u8FSM_currState = APPSTATE_FIRST_ENTER;
            }
            else
            {
                /* keep state */
            }
            break;
/**********************************************/            
/******/case APPSTATE_FIRST_ENTER:/***********/ 
/********************************************/
            if(UNDEFINED == bIamFirst)
            {
                bIamFirst = TRUE;
                sMyInfo.bIsOnMain = TRUE;               
            }
            else if(FALSE == bIamFirst)
            {
                /* find bIsOnMain */
                if( sMyInfo.u8RoadNum == getOpositeRoad(sCarsInfo[0].u8RoadNum) )
                {
                    /* get the same value */
                    sMyInfo.bIsOnMain = sCarsInfo[0].bIsOnMain;
                }
                else
                {
                    /* get the opposite value */ 
                    sMyInfo.bIsOnMain = sCarsInfo[0].bIsOnMain ^ 0x01;
                }
            }
            else /* flag is TRUE or smth else */
            {
                /* => something wrong happened. */
            }
                
            if(DIR_UNDEF == sMyInfo.u8Direction)
            {   
                //sMyInfo.u8Direction = getRandomDir();
                sMyInfo.u8Direction = DEBUG_DIR;          
            }
            else
            {
                /* u8Direction has been set elsewhere. */
            }
            
            T_U8 u8Message = 0x00;
            u8Message |= COM_MSG__FIRST_ENTER;
            if(0 != sMyInfo.u8RoadNum)
            {
                u8Message |= (sMyInfo.u8RoadNum     << 2);
            }
            if(0 != sMyInfo.u8Direction)
            {   
                u8Message |= (sMyInfo.u8Direction   << 4);    
            }
            if(FALSE != sMyInfo.bIsOnMain)
            {
                u8Message |= (sMyInfo.bIsOnMain     << 6);
            }
            
            COM_vSendMessage(u8Message);
            
            /* Update state */
            u8FSM_precState = u8FSM_currState;
            u8FSM_currState = APPSTATE_BEFORE_STOP;
            break;
/**********************************************/            
/******/case APPSTATE_BEFORE_STOP:/***********/
/********************************************/              
            DCMOT_vSetDirSpeed(FORWARD, APP_SPEED__BEFORESTOP);
            
            if(TRUE == bStopLine)
            {
                bStopLine = FALSE;
                
                /* Update state */
                u8FSM_precState = u8FSM_currState;
                u8FSM_currState = APPSTATE_STOP_LINE;
            }
            else
            {
                /* keep state */
            }
            break;
/**********************************************/              
/******/case APPSTATE_STOP_LINE:/*************/
/********************************************/            
            DCMOT_vSetDirSpeed(FORWARD, 0);
            
            T_U8 u8Index = 0;
            BOOL bCanGo = UNDEFINED;
            
            if(TRUE == sMyInfo.bIsOnMain)
            {
                /* parcurge pana la cel de pe drumul principal */
                while(u8Index < u8NumSavedCars && FALSE == sCarsInfo[u8Index].bIsOnMain)
                {
                    ++u8Index;
                }
                
                /* if the other car on main road NOT FOUND. */
                if(u8Index == u8NumSavedCars)
                {
                    bCanGo = TRUE;
                }
                else    /* FOUND the other car on main road */
                {
                    /* Compute priority */
                    if(DIR_LEFT == sMyInfo.u8Direction && DIR_LEFT != sCarsInfo[u8Index].u8Direction)
                    {
                        bCanGo = FALSE;
                    }
                    else
                    {
                        bCanGo = TRUE;
                    }
                }
            }
            else    /* NOT on Main Road */
            {
                /* if there are no other cars */
                if(0 == u8NumSavedCars)
                {
                    bCanGo = TRUE;
                }
                else
                {
                    /* check if anyone is on Main Road */
                    while(u8Index < u8NumSavedCars && FALSE == sCarsInfo[u8Index].bIsOnMain)
                    {
                        ++u8Index;
                    }

                    /* if no car is on Main Road */
                    if(u8Index == u8NumSavedCars)
                    {
                        /* it means there is just 1 other car. This car is not on the Main Road. */
                        /* dupa cum e gestionat vectorul, ea ar trebui sa fie pe pozitia [0]. */
                        
                        /* Compute priority */
                        if(DIR_LEFT == sMyInfo.u8Direction && DIR_LEFT != sCarsInfo[0].u8Direction)
                        {
                            bCanGo = FALSE;
                        }
                        else
                        {
                            bCanGo = TRUE;
                        }
                    }
                    else    /* there is a car on Main Road */
                    {
                        bCanGo = FALSE;
                        /* and wait for it to cross the junction. */
                    }
                }
            }
            
            if(TRUE == bCanGo)
            {
                /* Update state */
                u8FSM_precState = u8FSM_currState;
                u8FSM_currState = APPSTATE_CROSS;
            }
            else if(FALSE != bCanGo)
            {
                /* not good */
            }
            else 
            {
                /* keep state */
            }
            break;
/**********************************************/              
/******/case APPSTATE_CROSS:/*****************/
/********************************************/            
            DCMOT_vSetDirSpeed(FORWARD, APP_SPEED__CROSS);     
                   
            if(TRUE == bWhite)
            {
                T_U8 u8Message = 0x00;
                u8Message |= COM_MSG__EXIT;
                if(0 != sMyInfo.u8RoadNum)
                {
                    u8Message |= (sMyInfo.u8RoadNum << 2);
                }
                COM_vSendMessage(u8Message);                
                
                u8FSM_precState = u8FSM_currState;
                u8FSM_currState = APPSTATE_FINISH;
            }
            
            /*
            if(FALSE == bEncoderSet)
            {
                switch(sMyInfo.u8Direction)
                {
                    case DIR_FWD:
                        u16EncoderPathLength = ENC_PATH_LEN_FWD;
                        break;
                    case DIR_LEFT:
                        u16EncoderPathLength = ENC_PATH_LEN_LEFT;
                        break;
                    case DIR_RIGHT:
                        u16EncoderPathLength = ENC_PATH_LEN_RIGHT;
                        break;
                }
                
                bEncoderSet = TRUE;
            }
              
            u16EncoderPassed += QEI_s16getElapsed();
            
            if(u16EncoderPassed > u16EncoderPathLength)
            {             
                T_U8 u8Message;
                u8Message = COM_MSG__EXIT;
                u8Message |= (sMyInfo.u8RoadNum << 2);
                COM_vSendMessage(u8Message); 
                      
                bExecCurve = FALSE;
                
                u8FSM_precState = u8FSM_currState;
                u8FSM_currState = APPSTATE_FINISH;
            }
            else
            {
                
            }
            */
            break;
/**********************************************/        
/******/case APPSTATE_OBST_DETECT:/***********/    
/********************************************/            
            DCMOT_vSetDirSpeed(FORWARD,0);
            /* Car stops. See function vObstLogic(). */
            break;
/**********************************************/   
/******/case APPSTATE_FINISH:/****************/
/********************************************/
            DCMOT_vSetDirSpeed(FORWARD,0);
            /* Car STOP. */
            break;
/**********************************************/          
/******/default:/*****************************/
/********************************************/
            /* undefined state. should NOT get here. */
            break;
    }
}