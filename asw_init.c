#include "general.h"
#include "hal_lineFollower.h"
#include "asw_appLogic.h"
#include "asw_fsm.h"

/* hal_lineFollower.c */
extern T_U8 u8ServoAngle;

/* asw_appLogic.c */
extern carInfo sCarsInfo[APP_MAX_NUM_CARS];
extern carInfo sMyInfo;
extern T_U8 u8NumSavedCars;
extern BOOL bIamFirst;

extern T_U8 u16EncoderPathLength;
extern T_U8 u16EncoderPassed;
extern BOOL bEncoderSet;

extern T_U8 u8FSM_currState;
extern T_U8 u8FSM_precState;


void ASW_vInit()
{
    u8ServoAngle = ANGLE_FRONT;
    
    sMyInfo.u8RoadNum = 0;
    sMyInfo.u8Direction = DIR_UNDEF;
    sMyInfo.bIsOnMain = UNDEFINED;
    
    u8NumSavedCars = 0;
    bIamFirst = UNDEFINED;
    
    u16EncoderPassed = 0;
    u16EncoderPathLength = 0;
    bEncoderSet = FALSE;
    
    u8FSM_currState = APPSTATE_FOLLOW_LINE;
    u8FSM_precState = APPSTATE_FOLLOW_LINE;
    
    // LED on.
    //GPIO_u8WritePortPin(PORT_A, 10, 1);
    
    /* connected to LEDs */
    _TRISA8 = 0;
    _TRISB8 = 0;
    _TRISC7 = 0;
    
    LEDp_OFF
    LEDn_OFF
    LEDa_OFF
    LEDr_OFF
}
