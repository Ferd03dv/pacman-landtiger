/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdbool.h> // Per bool
#include <stdio.h> /*for sprintf*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "GLCD/GLCD.h" 
#include "../joystick.h"
#include "../CAN.h"



/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


int spCouter = 0;

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

int gameStatus = 1;	// 1 - primo movimento non effettuato / 0 - game in corso / -1 - game terminato

extern int causualValuesTime[6];
extern int nPills;
extern uint16_t score;
extern uint8_t vite;
extern int posPacManX;
extern int posPacManY;
extern int posGhostX;
extern int posGhostY;
extern int newPacmanMap[29][30];

char countString[8];

int bestMove = -1;
extern uint8_t count;	
extern uint8_t viteCAN;
void TIMER0_IRQHandler (void)
{
	
	int j=0;
	static int clear = 0;
	
	uint8_t halfScore1 = (uint8_t)score;
	uint8_t halfScore2 = ((uint8_t)(score >> 8));
	
	
	CAN_TxMsg.data[0] = count;
	CAN_TxMsg.data[1] = vite;
	CAN_TxMsg.data[2] = halfScore1;
	CAN_TxMsg.data[3] = halfScore2;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg (1, &CAN_TxMsg);             			
	clear = 0;
	
	pacmanMovement();
	

  LPC_TIM0->IR = 1;			// clear interrupt flag

		return;
	
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{	
  if (gameStatus == 0) {
		count--;
		sprintf(countString, "%d ", count);
	}
	
	if(spCouter < 6){
		int j=0;
		for (j = 0; j < 6; j++) {
				if (causualValuesTime[j] == count) {
						generateCasualPills();
						spCouter++;
				}
		}
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler(void)
{
	
	static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;

		
    LPC_TIM2->IR = 1;  // Pulisci il flag di interruzione
    return;
}

void TIMER3_IRQHandler(void)
{
	
	disable_timer(2);

		
    LPC_TIM3->IR = 1;  // Pulisci il flag di interruzione
    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
