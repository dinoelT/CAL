
#include "hal_dc_motor.h"
#include "mcal_pwm.h"


/*******************************************************************************
*  Function name    : DCMOT_vInit
*  Description      :1.Initialize PWM1
*                    2.Set pin RA9 as digital output 
*  List of arguments: no arguments 
*  Return value     : no return value
********************************************************************************/
void DCMOT_vInit(void)
{
   PWM1_vInit();
   GPIO_u8SetPortPin(PORT_A,9,DIGITAL,OUTPUT);  
}

/*******************************************************************************
*  Function name    : DCMOT_vSetDirSpeed
*  Description      : 1) Compare current speed with target speed 
*                     2)Accelerate/Decelerate until desired speed is reached   
*  List of arguments: f16Speed-> Speed (from 0 to 100)
*                     u8Dir: choose between FORWARD/BACKWARD
*  Return value     : no return value
*******************************************************************************/
void DCMOT_vSetDirSpeed( T_U8 u8Dir, T_F16 f16Speed)
{
    static T_F16 f16LastSpeed=0;
    static T_F16 f16CurrSpeed=0;
    T_F16 f16DutyCycle;
    
    if(u8Dir == BACKWARD)
    {
        /* Make value negative (for smooth forward->backward motion transition) */
        f16Speed*= -1;
    }
    
    if(f16Speed != f16LastSpeed)
    {
        if(f16LastSpeed < f16Speed)
        {
            f16CurrSpeed += ACCELER;
        }
        else
        {
            f16CurrSpeed -= DECELER;
        }
        
        if(f16CurrSpeed < 0)
        {
            u8Dir = BACKWARD;
            /* Make f16DutyCycle positive */
            f16DutyCycle = f16CurrSpeed * -1;
        }
        else
        {
            u8Dir = FORWARD;
            f16DutyCycle = f16CurrSpeed;
        }
        
        /* Update f16LastSpeed */
        f16LastSpeed = f16CurrSpeed;
        
        /* Set DC Motor direction and duty cycle */
        GPIO_u8WritePortPin(PORT_A,9,u8Dir);
        PWM1_vSetDuty(f16DutyCycle,2);
    }
    else
    {
        /* Do nothing */
    }   
}
