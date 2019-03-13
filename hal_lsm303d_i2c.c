
#include "hal_lsm303d_i2c.h"
#include "mcal_i2c.h"
#include <math.h>


/*  accelerometer readings */
Vector aAccVal;
/* magnetometer readings */
Vector aMgnVal; 

/*******************************************************************************
*  Function name    : LSM303D_s8ReadRegister
*  Description      : Using I2c communication we forward the address of the 
*                     registry  by LSM303D then it return the value witch is 
*                     stored in that register                      
*  List of arguments: address register that we want to read 
*  Return value     : return data stored in that register
********************************************************************************/
T_S8 LSM303D_s8ReadRegister(T_S8 s8Address)
{
    T_S8 Data;
    I2C_s8ReadReg(ADD,s8Address,&Data);
   
	return (Data);			
}
/*******************************************************************************
*  Function name    : LSM303D_s8WriteRegister
*  Description      : Using I2c communication we forward the address of the 
*                     registry  by LSM303D then the value will be write in it
*                                    
*  List of arguments: address register that we want to write in it
*  Return value     : no return value
********************************************************************************/
void LSM303D_s8WriteRegister(T_S8 s8Address, T_S8 s8Value)
{
    I2C_s8WriteReg(ADD,s8Address,s8Value);
    
}
/*******************************************************************************
*  Function name    : LSM303D_vInit
*  Description      : registers initialization 
*                                    
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void LSM303D_vInit(void)
{    
    /* initialization the I2C for to communicate with LSM303D */
    I2C_vInit();
    LSM303D_vInit_MinMaxMag();
    /* for calibration the magnetometer */
   LSM303D_vInit_MinMaxMag();
    
    /* Accelerometer */

	/* 0x57 = 0b01010111 */
    /* AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled) */
    LSM303D_s8WriteRegister(CTRL1, 0x57);

    /* 0x00 = 0b00000000 */
    /* AFS = 0 (+/- 2 g full scale) */
    LSM303D_s8WriteRegister(CTRL2, 0x00);

    /* Magnetometer */

    /* 0x64 = 0b01100100 */
    /* M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR) */
    LSM303D_s8WriteRegister(CTRL5, 0x64);

    /* 0x20 = 0b00100000 */
    /* MFS = 01 (+/- 4 gauss full scale) */
    LSM303D_s8WriteRegister(CTRL6, 0x20);

    /* 0x00 = 0b00000000 */
    /* MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode) */
    LSM303D_s8WriteRegister(CTRL7, 0x80);
}
 /*******************************************************************************
*  Function name    :  LSM303D_vReadAcc
*  Description      : Reads the 3 accelerometer channels and stores them 
*                     in vector a
*                                    
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/  
void LSM303D_vReadAcc(void)
{
	T_U8 xla, xha, yla, yha, zla, zha;

	xla = LSM303D_s8ReadRegister(OUT_X_L_A);
	xha = LSM303D_s8ReadRegister(OUT_X_H_A);
	yla = LSM303D_s8ReadRegister(OUT_Y_L_A);
	yha = LSM303D_s8ReadRegister(OUT_Y_H_A);
	zla = LSM303D_s8ReadRegister(OUT_Z_L_A);
	zha = LSM303D_s8ReadRegister(OUT_Z_H_A);

	/* combine high and low bytes - each accelerometer channel have 16-bit resolution */
	aAccVal.x = (T_S16)xha  << 8 | xla;
	aAccVal.y = (T_S16) yha << 8 | yla;
	aAccVal.z = (T_S16) zha << 8 | zla;
}
/*******************************************************************************
*  Function name    :  LSM303D_vReadMag
*  Description      : Reads the 3 magnetometer channels and stores them 
*                     in vector m
*                                    
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void LSM303D_vReadMag(void)
{
	T_U8 xlm, xhm, ylm, yhm, zlm, zhm;

	xlm = LSM303D_s8ReadRegister(OUT_X_L_M);
	xhm = LSM303D_s8ReadRegister(OUT_X_H_M);
	ylm = LSM303D_s8ReadRegister(OUT_Y_L_M);
	yhm = LSM303D_s8ReadRegister(OUT_Y_H_M);
	zlm = LSM303D_s8ReadRegister(OUT_Z_L_M);
	zhm = LSM303D_s8ReadRegister(OUT_Z_H_M);

	/* combine high and low bytes - each magnetometer channel have 16-bit resolution */
	aMgnVal.x = (T_S16)xhm << 8 | xlm;
	aMgnVal.y = (T_S16)yhm << 8 | ylm;
	aMgnVal.z = (T_S16)zhm << 8 | zlm;
}
/*******************************************************************************
*  Function name    : LSM303D_vRead
*  Description      : Reads all 6 channels of the LSM303 and stores them in the 
 *                    object variables 
*                     Reads the 3 accelerometer channels  
*                     Reads the 3 magnetometer channels
*                                    
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void LSM303D_vRead(void)
{
  LSM303D_vReadAcc();
  LSM303D_vReadMag();
}

/* variable for minim and maximum readings magnetometer */
Vector aMgnVal_min ,aMgnVal_max;

/*******************************************************************************
*  Function name    : LSM303D_vInit_MinMaxMag
*  Description      : First time the min is a maximum number and max is a 
*                     minimum number
*                     After we done to calculate min and max for each axis 
*                     we will return to modify min and max values.                
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void  LSM303D_vInit_MinMaxMag(void)
  {
      if(CAR == 1)
      {   
     aMgnVal_min.x = -699;
     aMgnVal_min.y = -1235;
     aMgnVal_max.x = 1258;
     aMgnVal_max.y = 757;
     aMgnVal_min.z = -2549;
     aMgnVal_max.z = -2183;
      }
      if(CAR == 2)
      {
     aMgnVal_min.x = 883;
     aMgnVal_min.y = -853;
     aMgnVal_max.x = 3045;
     aMgnVal_max.y = 1144;
     aMgnVal_min.z = -1918;
     aMgnVal_max.z = -1756;
      }
      
  }
/*******************************************************************************
*  Function name    : LSM303D_vMinMaxMag
*  Description      : Each car has a calibration and for this is necessary to 
*                     calculate minimum an maximum value for each axis
*                     In this case the values were calculated turning the car 360
*                     degrees               
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void LSM303D_vMinMaxMag(void)
{
    
    LSM303D_vReadMag();
    if(aMgnVal.x > aMgnVal_max.x)
        aMgnVal_max.x = aMgnVal.x;
    if(aMgnVal.x <= aMgnVal_min.x)
        aMgnVal_min.x = aMgnVal.x;
    if(aMgnVal.y > aMgnVal_max.y)
        aMgnVal_max.y = aMgnVal.y;
    if(aMgnVal.y <= aMgnVal_min.y)
        aMgnVal_min.y = aMgnVal.y;
    if(aMgnVal_max.z < aMgnVal.z)
        aMgnVal_max.z = aMgnVal.z;
    if(aMgnVal_min.z >= aMgnVal.z)
        aMgnVal_min.z = aMgnVal.z;
        
}
/*******************************************************************************
*  Function name    : LSM303D_vMinMaxMag
*  Description      : In this function we are calculating values of each axis 
*                     calibrate and then heading             
*  List of arguments: no arguments
*  Return value     : return angle  
********************************************************************************/
T_F16 LSM303D_f16Heading(void)
{
    LSM303D_vRead();
    
    Vector offset_Mgn;
    
   /* subtract offset (average of min and max) from magnetometer readings */
    offset_Mgn.x = (aMgnVal_min.x +aMgnVal_max.x) / 2;
    offset_Mgn.y= (aMgnVal_min.y +aMgnVal_max.y) / 2;
    offset_Mgn.z = (aMgnVal_min.z +aMgnVal_max.z) / 2;
    
   /*calibrate magnetometer readings */
    aMgnVal.x-=offset_Mgn.x;
    aMgnVal.y-=offset_Mgn.y; 
    aMgnVal.z-=offset_Mgn.z;
    
    /************************************************************************** 
     * calculate pitch and roll
     * 
     * float pitch,roll;
     * 
     * pitch = asin(-(aAccVal.x*0.000061));
     * roll  = asin(aAccVal.y*0.000061/cos(pitch));
    
     * aMgnVal.x =aMgnVal.x*cos(pitch)+aMgnVal.z*sin(pitch);
     * aMgnVal.y =aMgnVal.x*sin(roll)*sin(pitch)+aMgnVal.y*cos(roll)-aMgnVal.z*sin(roll)*cos(pitch);
     **************************************************************************/
    
   /* compute heading */
    T_F16 heading;
    heading = atan2(aMgnVal.y,aMgnVal.x);
       if(heading < 0)
           heading += 2 * PI;
       heading=(heading * 180/PI);
    return heading;
  }
  



