

#include "hal_init.h"

/*******************************************************************************
*  Function name    :  HAL_vInit
*  Description      : Initializes all Modules         
*  List of arguments: no arguments  
*  Return value     : no return value
********************************************************************************/
void HAL_vInit()
{
    DCMOT_vInit();
    RF_vInit();
}
