/* 
 * File:   sys_tasks.h
 * Author: Cristian T. A.
 *
 * Created on August 17, 2018, 1:26 PM
 */

#include "general.h"
#include "sys_tasks.h"

#include "mcal_init.h"
#include "hal_init.h"
#include "asw_init.h"
#include "asw_com.h"

#include "asw_fsm.h"

extern BOOL bObstDetected;

void TASK_Inits()
{
    MCAL_vInit();
    HAL_vInit();
    ASW_vInit();
}

void TASK_1ms()
{
    COM_vStartListening();
    COM_vCheckIRQ();
    COM_vProcessFIFO();
}

void TASK_5ms()
{
    //RF_vCheckIRQ();
    /****************/
    
    APP_FSM();
}

void TASK_10ms()
{   
    //test_vReceiver();
    /*****************/
    if(FALSE == bObstDetected)
    {
        vDoLineFollower();
    }
}

void TASK_100ms()
{ 
    
}

void TASK_500ms()
{ 
    //test_vTransmitter();
    /********************/
}

void TASK_1000ms()
{

}