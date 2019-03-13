/* 
 * File:   hal_battery.h
 * Author: uia94881
 *
 * Created on August 16, 2018, 10:35 AM
 */

#ifndef HAL_BATTERY_H
#define	HAL_BATTERY_H

#include "general.h"
#include "mcal_adc.h"

#define     NR_OF_ADC_READINGS  5
#define     MAX_RAW_VAL_12_BIT  4095
#define     ADC_VOLT_REF        3.06
#define     VOLT_DIV_RATIO      4
#define     MIN_BATT_VOLT        6
#define     MAX_BATT_VOLT        8.3
#define     MAX_CHARGE          100
#define     MIN_CHARGE          0


/* Battery function declaration */
T_U16 f16ReadBatt(void);

#endif	/* HAL_BATTERY_H */

