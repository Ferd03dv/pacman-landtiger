/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include <stdio.h>
#include "../pacmanFunction.h"
#include "music/music.h"

#define BLOCK_SIZE 8   // Dimensione di ogni blocco in pixel
#define OFFSET_TOP 48  // Offset verticale per lo spazio riservato alle statistiche
#define OFFSET_LEFT 0  // Offset orizzontale (se necessario)


extern int newPacmanMap[29][30];
extern int posPacManX;
extern int posPacManY;
extern int posGhostX;
extern int posGhostY;
extern int move;
extern int moveGhost;
extern uint8_t score;
extern int nPills;
int superpillTaken = 0; //0 no, 1 si

void moveStop() {
	move = 0;
}

void moveUp() {
	int blockUp = newPacmanMap[posPacManY-1][posPacManX];

	if (blockUp == 16) {
		moveStop();
		return;
	}
	
	updatePacManPosition(1, 0, 0, 1);
	
	if (blockUp == 17) {
		
		addPoints(10);
	} else if (blockUp == 18) {
		addPoints(50);
	}
}

void moveRight() {
	int blockRight = newPacmanMap[posPacManY][posPacManX+1];

	if (blockRight == 16) {
		moveStop();
		return;
	}
	
	if (blockRight == 20) {
		updatePacManPosition(27, 0, 1, 0);
		return;
	}
	
	updatePacManPosition(1, 1, 1, 0);
	
	// Aumenta lo score
	if (blockRight == 17) {	
		addPoints(10);
	} else if (blockRight == 18) {
		NOTE nota;
		nota.freq=c5,
		nota.duration = time_semiminima;
		playNote(nota);
		
		addPoints(50);
	}
}

void moveLeft() {
	int blockLeft = newPacmanMap[posPacManY][posPacManX-1];

	if (blockLeft == 16) {
		moveStop();
		return;
	}
	
	if (blockLeft == 20) {
		updatePacManPosition(27, 1, 1, 0);
		return;
	}
	
	updatePacManPosition(1, 0, 1, 0);
	
	if (blockLeft == 17) {
		addPoints(10);
	} else if (blockLeft == 18) {
		addPoints(50);
	}
}

void moveDown() {
	int blockDown = newPacmanMap[posPacManY+1][posPacManX];

	if (blockDown == 16 || blockDown == 21) {
		moveStop();
		return;
	}
	
	updatePacManPosition(1, 1, 0, 1);
	
	if (blockDown == 17) {
		addPoints(10);
	} else if (blockDown == 18) {
		addPoints(50);
	}
}


void moveGhostUp(int ric){
	
	if(ric==0){
		updateGhostPosition(0, 0, 1);
	}else if(ric==1){
		updateGhostPosition(0, 0, 1);
		moveGhostUp(0);
	}
	
}

void moveGhostDown(){
	
	updateGhostPosition(1, 0, 1);
	
}

void moveGhostLeft(){
	
	updateGhostPosition(0, 1, 0);
}

void moveGhostRight(){
	
	updateGhostPosition(1, 1, 0);
	
}

