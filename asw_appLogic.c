/* 
 * File:   asw_appLogic.c
 * Author: Cristian T. A.
 *
 * Created on September 18, 2018, 2:18 PM
 */

#include "hal_lineFollower.h"
#include "mcal_encoder.h"
#include "asw_appLogic.h"

carInfo sCarsInfo[APP_MAX_NUM_CARS];
carInfo sMyInfo;
T_U8 u8NumSavedCars;
BOOL bIamFirst;

T_U16 u16EncoderPathLength;
T_U16 u16EncoderPassed;
BOOL bEncoderSet;

/*******************************************************************************
*  Function name    : getOpositeRoad
*  Description      : 1) Returns the road index of the car on the opposite side             
*  List of arguments: T_U8 myIndex -> The index of the road you are on
*  Return value     : T_U8 -> Index of the Road/Car on the opposite side
********************************************************************************/
T_U8 getOpositeRoad(T_U8 myIndex)
{
    if(myIndex > 1)
    {
        return myIndex-2;
    }
    else
    {
        return myIndex+2;
    }
}

/*******************************************************************************
*  Function name    : getRandomDir
*  Description      : 1) Assign a random value to u8Dir(Direction of movement)             
*  List of arguments: No arguments
*  Return value     : T_U8 -> The direction (0/1/2) for right, left, forward
********************************************************************************/
T_U8 getRandomDir()
{
    return (1 + QEI_u16getCount() % 3);
}

    
