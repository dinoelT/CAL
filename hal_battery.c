#include "hal_battery.h"


/*******************************************************************************
*  Function name    : f16ReadBatt
*  Description      : Calculates the average Raw value of 5 ADC readings
*                     Converts Raw value into Voltage
*                     Calculates the Charge of the battery in %
*  List of arguments: no arguments
*  Return value     : T_U16 -> Charge in %.
********************************************************************************/
T_U16 f16ReadBatt()
{
    T_U16 u16ADC_Result = 0;
    T_F16 f16BattVolt;
    T_U16 u16Charge;
    static T_U8 u8index;	
    /* Sum 5 ADC readings of the battery */
    for(u8index = 0; u8index < NR_OF_ADC_READINGS; u8index++)
    {
        u16ADC_Result += ADC_u16Read(0);
    }
    /* Calculate the average */
    u16ADC_Result /= NR_OF_ADC_READINGS;
    
    /* Convert Raw value into Voltage */
    f16BattVolt = (ADC_VOLT_REF * u16ADC_Result ) / MAX_RAW_VAL_12_BIT;
    
    /* The battery voltage is fed into the ADC through a 4:1 Voltage Divider */
    f16BattVolt *= ADC_VOLT_REF;

    /* Voltage Lookup Table */  
    static const T_F16 VLT[2][4] =
    /* Volts on battery */
    {{ 8.3,   7,   6.5,   6 },
    /* Charge in % */
	{  100,   20,  10,    0 }};	
    
    
    /* Initially we suppose u16BatteryVoltage is not found in any interval */
	BOOL u8isInInterval = FALSE;
    
    /* Find the interval of the Lookup Table in which the Raw Value is found */
    for (u8index = 0; u8index < 3; u8index++)
    {
        static T_F16 f16SupLimVolt;
        static T_F16 f16InfLimVolt;
        /* Update Limit Notations */
        f16SupLimVolt = VLT[0][u8index];
        f16InfLimVolt = VLT[0][u8index + 1];

        if ((f16BattVolt <= f16SupLimVolt) && (f16BattVolt > f16InfLimVolt))
        {
            static T_F16 f16deltaVolt;
            static T_F16 f16Resolution;
            static T_U16 u16InfLimCharge;

            /* Update all formula notations */
            f16deltaVolt  = f16BattVolt- f16InfLimVolt;
            f16Resolution = (VLT[1][u8index] - VLT[1][u8index + 1])
                            /(VLT[0][u8index] - VLT[0][u8index + 1]);
            u16InfLimCharge = VLT[1][u8index + 1];

            /***************** Calculate the Sensor Distance!!! ****************/
             u16Charge= u16InfLimCharge + f16deltaVolt * f16Resolution;
            
            /* f16BatteryVoltage WAS FOUND in one of the intervals */
            u8isInInterval = TRUE;
        }
    }
    /***************************************************************************
     *                                              
     * BatteryCharge = InferiorLimitCharge  DeltaVoltage * Resolution
     * 
     * DeltaRaw = BatteryVoltafe - Inferior_Voltage_Limit
     * 
     *                 Length_of_Charge_Interval   
     * Resolution =  ------------------------------
     *                  Length_of_Voltage_Interval
     *                            
     ***************************************************************************/
    
    /* If f16BatteryVoltage was not found in any of the intervals */
    if(u8isInInterval == FALSE)
    {
        /* If f16BatteryVoltage is less than the minimum battery voltage */
        if(f16BattVolt < MIN_BATT_VOLT)
        {
            /* Set u16Charge at the defined minimal charge level */
            u16Charge = MIN_CHARGE;
        }
        else
        {
            /* If f16BatteryVoltage is greater than the maximum raw value */
            if(f16BattVolt > MAX_BATT_VOLT)
            {
                /* Set u16Charge at the defined maximal charge level */
                u16Charge = MAX_CHARGE;
            }
        }     
    }
    /* Return the distance (in millimeters) */
    return u16Charge;  
}

