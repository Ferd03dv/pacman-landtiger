#include "button.h"
#include "LPC17xx.h"
#include "timer/timer.h"
#include "GLCD/GLCD.h" 

extern int down;
extern int move;
extern int bestMove;

extern int pauseGame;	
int lastMove;
int lastGhost;

void EINT0_IRQHandler (void)	  
{
	
	if (pauseGame == 0) {
		lastMove = move;
		lastGhost = bestMove;
		move = 0;
		pauseGame = 1;
		pause();
	} else {
		move = lastMove;
		bestMove = lastGhost;
		pauseGame = 0;
		resume();
	}
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	
{
	NVIC_DisableIRQ(EINT1_IRQn);		
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     
	down=1;
	LPC_SC->EXTINT &= (1 << 1);    
}

void EINT2_IRQHandler (void)	  	
{
	
  LPC_SC->EXTINT &= (1 << 2);        
}
