/* 
 * File:   rte.h
 * Author: uia94848
 *
 * Created on October 18, 2018, 12:21 PM
 */

#ifndef RTE_H
#define	RTE_H

#include "hal_nRF.h"

#define RTE_RF_vCheckIRQ()                          (RF_vCheckIRQ())
#define RTE_RF_vBeginTransmit(u8Message)            (RF_vBeginTransmit(u8Message))
#define RTE_RF_vBeginReceive()                      (RF_vBeginReceive())

#define RTE_DCMOT_vSetDirSpeed(U8Dir, f16Speed)     (DCMOT_vSetDirSpeed(u8Dir, f16Speed))

#define RTE_APP_FSM()                               (APP_FSM())

#define RTE_vDoLineFollower()                       (vDoLineFollower()) 


#endif	/* RTE_H */

