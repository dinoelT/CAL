#include "hal_servomotor.h"
#include "mcal_pwm.h"

/*******************************************************************************
*  Function name    : vSetServoAngle
*  Description      : Shorter hight pulse than 1.5 ms moves it in the counter clockwise 
*                     direction toward the 0° position, and any longer than 
*                     1.5 ms will turn the servo in a clockwise direction toward 
*                     the 180° position. 
*                     For a PWM with period = 20ms and duty between 5.2 and 8.8
*                     result a hight pulse between 1.5ms and 2ms
*  List of arguments: angle between 0 to 180 but is recommended to introduce un 
*                     angle with value between 30 to 150 due to the architecture 
*                     car
*  Return value     : no return value
********************************************************************************/
void vSetServoAngle(T_F16 f16Angle)
{
    if(f16Angle>=ANGLE_MIN&&f16Angle<=ANGLE_MAX)
    {
      
        /* to avoid overloading */  
        if(f16Angle>LIMIT_SUP)
            f16Angle=LIMIT_SUP;
      
        /* to avoid overloading */ 
        if(f16Angle<LIMIT_INF)
            f16Angle=LIMIT_INF;
      
        T_F16 f16Duty;
        f16Duty = (f16Angle-LIMIT_INF)*GAIN+OFFSET;
      
        /* set pwm for servomotor (PWM1H1 = RB14 ) */
        PWM1_vSetDuty(f16Duty,1);
    }
     
}


