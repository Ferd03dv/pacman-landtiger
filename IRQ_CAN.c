/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include <stdio.h>

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg CAN_RxMsg;    /* CAN message for receiving */     

uint8_t countCAN;
uint8_t viteCAN;
uint8_t scoreCAN1;
uint8_t scoreCAN2;

uint16_t scoreCAN;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		

		countCAN = CAN_RxMsg.data[0];  
		viteCAN = CAN_RxMsg.data[1]; 
		scoreCAN1 = CAN_RxMsg.data[2];
		scoreCAN2 = CAN_RxMsg.data[3];
		
		scoreCAN = ((uint16_t)scoreCAN2 << 8) |  scoreCAN1;

		char countString[8];
		sprintf(countString, "%d ", countCAN);
		GUI_Text(20, 20, (uint8_t *) countString, White, Black);
		
		char countString2[16];
		sprintf(countString2, "%d ", scoreCAN);
		GUI_Text(190, 20, (uint16_t *) countString2, White, Black);
		
		
		
		uint32_t combined = (uint32_t)countCAN | ((uint32_t)viteCAN << 8) | ((uint32_t)scoreCAN << 16);
	}
}
