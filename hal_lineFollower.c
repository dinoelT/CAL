#include "hal_lineFollower.h"
#include "hal_servomotor.h"
#include "hal_dc_motor.h"
#include "asw_appLogic.h"
#include "asw_fsm.h"

extern carInfo sMyInfo;
extern T_U8 u8FSM_currState;

T_U8 u8ServoAngle;
/* Movement direction */
T_U8 u8LastVal;

/* exported */
/* flag indicating if the module LineFollower identified the Stop Line. */
BOOL bStopLine;

BOOL bWhite;

/*******************************************************************************
*  Function name    : vSetDirLineFollower
*  Description       1.Set RC0, RC1, RC2, RC3, RC4, RC5 digital outputs or 
*                     inputs  pins                    
*  List of arguments: u16SetIntOut (it can be 1 or 0) 
*  Return value     : no return value
********************************************************************************/
void vSetDirLineFollower(BOOL u16SetIntOut)
{
    GPIO_u8SetPortPin(PORT_C,0,DIGITAL,u16SetIntOut);
    GPIO_u8SetPortPin(PORT_C,1,DIGITAL,u16SetIntOut); 
    GPIO_u8SetPortPin(PORT_C,2,DIGITAL,u16SetIntOut);
    GPIO_u8SetPortPin(PORT_C,3,DIGITAL,u16SetIntOut);
    GPIO_u8SetPortPin(PORT_C,4,DIGITAL,u16SetIntOut);
    GPIO_u8SetPortPin(PORT_C,5,DIGITAL,u16SetIntOut);
}


/*******************************************************************************
*  Function name    : vDoWriteLineFollower
*  Description       1.Write in RC0, RC1, RC2, RC3, RC4, RC5 which  charge the 
*                      capacitors                                        
*  List of arguments: -
*  Return value     : no return value
********************************************************************************/
void vDoWriteLineFollower(void)
{
    vSetDirLineFollower(OUTPUT);
    
    GPIO_u8WritePortPin(PORT_C,0,1);
    GPIO_u8WritePortPin(PORT_C,1,1);
    GPIO_u8WritePortPin(PORT_C,2,1);
    GPIO_u8WritePortPin(PORT_C,3,1);
    GPIO_u8WritePortPin(PORT_C,4,1);
    GPIO_u8WritePortPin(PORT_C,5,1);
}

/*******************************************************************************
*  Function name    : u8GetValueLineFollower
*  Description       1.Write in RC0, RC1, RC2, RC3, RC4, RC5 which  charge the 
*                      capacitors
*                    2.Wait minim 20 microseconds until all capacitors are charged
*                    3.Set pins as digital input
*                    4.Wait until the capacitors are discharged (if after 250 us
*                      the capacitor corresponding each infrared led it is not 
*                      discharged it means that the infrared led receptor is on a
*                      black surface else the surface is white                      
*  List of arguments: no arguments 
*  Return value     : return lineFollower sensor(each bit meet with an infrared 
*                     led , LED2-bit 1,LED3-bit 2,LED4-bit 3 ,LED5-bit 4,LED6-bit 5
*                     LED7-bit 6 ).  
********************************************************************************/
T_U8 u8GetValueLineFollower(void)
{
    vDoWriteLineFollower();
    
    __delay_us(20);
    
    vSetDirLineFollower(INPUT);

    __delay_us(250);
     
    return GPIO_u16ReadPort(PORT_C)&63;   
}

/*******************************************************************************
*  Function name    : vDoLineFollower
*  Description      : 1)Read line follower sensor
*                     2)Depending on the read value choose an angle                       
*  List of arguments: -
*  Return value     : -
********************************************************************************/
void vDoLineFollower(void)
{
    T_U8 u8Val = u8GetValueLineFollower();
    /* only first 6 bits are used. */
    switch(u8Val)
    {
        case 0b000001:    /* 0x01 */
            u8ServoAngle = ANGLE_FRONT+30;
            break;
            
        case 0b000011:    /* 0x03 */   
        case 0b000111:    /* 0x07 */        
            u8ServoAngle = ANGLE_FRONT+20;
            break;
            
        case 0b000010:    /* 0x02 */
            u8ServoAngle = ANGLE_FRONT+15;
            break;
            
            
        case 0b001110:    /* 0x0E */       
        case 0b000110:    /* 0x06 */
            u8ServoAngle = ANGLE_FRONT+10;
            break; 
        
        case 0b111100:    /* 0x3C */    
        case 0b000100:    /* 0x04 */
            u8ServoAngle = ANGLE_FRONT+5;
            break;
               
        case 0b001100:    /* 0x0C */
            u8ServoAngle = ANGLE_FRONT;
            break;
            
        case 0b001000:    /* 0x08 */
            u8ServoAngle = ANGLE_FRONT-5;
            break; 
            
        case 0b111000:    /* 0x38 */
        case 0b011000:    /* 0x18 */
            u8ServoAngle = ANGLE_FRONT-10;
            break;
            
        case 0b010000:    /* 0x10 */
            u8ServoAngle = ANGLE_FRONT-15;
            break;
            
        case 0b110000:    /* 0x30 */
            u8ServoAngle = ANGLE_FRONT-20;
            break;
            
        case 0b100000:    /* 0x20 */
            u8ServoAngle = ANGLE_FRONT-30;
            break; 
            
        case 0b000000:    /* 0x00 */ 
            u8ServoAngle = ANGLE_FRONT;
            if(APPSTATE_CROSS == u8FSM_currState)
            {
                bWhite = TRUE;
            }
            if(0 != u8LastVal)
            {   
                if(APPSTATE_FOLLOW_LINE == u8FSM_currState)
                {
                    ++sMyInfo.u8RoadNum;
                    
                }
            }
            break;
            
        case 0b111111:    /* 0x3F */
            if(63 != u8LastVal)
            {
                bStopLine = TRUE;
            }
            break;
        /****************************/
            
        default:
            /* junction case */
            /* Keep your direction */
            if(APPSTATE_CROSS == u8FSM_currState)
            {
                switch(sMyInfo.u8Direction)
                {
                    case DIR_FWD:
                        u8ServoAngle = ANGLE_FRONT;
                        break;
                    case DIR_LEFT:
                        u8ServoAngle = ANGLE_LEFT;
                        break;
                    case DIR_RIGHT:
                        u8ServoAngle = ANGLE_RIGHT;
                        break;
                }
            }
            break;
    }
    
    vSetServoAngle(u8ServoAngle);
    u8LastVal = u8Val;
}
