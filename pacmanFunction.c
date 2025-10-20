/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 
#include "AsciiLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
static uint8_t NOWALL = 0;
static uint8_t WALL = 1;
static uint8_t STANDARD_PILL = 2;
static uint8_t POWER_PILL = 3;
static uint8_t PACMAN = 4;
int lastBlock = 15;
int move = 0;
int count = 60;
int lastScore = 0;
int causualValuesTime[6] = {-1, -1, -1, -1, -1, -1};
int ghostMoveCounter = 4;
int lastGhostPosX = -1;  // Posizione X dell'ultima posizione del fantasma
int lastGhostPosY = -1;  // Posizione Y dell'ultima posizione del fantasma

/* Private define ------------------------------------------------------------*/
#define NOWALL 			15
#define WALL 16
#define STANDARD_PILL 17
#define POWER_PILL 18
#define PACMAN 19
#define TELEPORT 20
#define GATE 21
#define GHOST 23
#define BLOCK_SIZE 8   // Dimensione di ogni blocco in pixel
#define OFFSET_TOP 48  // Offset verticale per lo spazio riservato alle statistiche
#define OFFSET_LEFT 0  // Offset orizzontale (se necessario)

/* Extern Variable ------------------------------------------------------------*/
extern int newPacmanMap[29][30];
extern uint8_t viteCAN;
extern int gameStatus;	
extern int nPills;
extern uint16_t score;
extern uint8_t vite;
extern int posPacManX;
extern int posPacManY;
extern int posGhostX;
extern int posGhostY;
extern int pauseGame;

void LCD_DrawBlock(uint8_t blockType, uint16_t posX, uint16_t posY){
	int i = 0;
	
	switch(blockType){
		case NOWALL:
		  for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			break;
		case WALL:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX+1, posY + i, posX + 7, posY + i, Blue);
			}
			break;
		case STANDARD_PILL:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			LCD_DrawLine(posX+4, posY+4, posX+4, posY+5, Grey);
			LCD_DrawLine(posX+5, posY+4, posX+5, posY+5, Grey);
			break;
		case POWER_PILL:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			LCD_DrawLine(posX+4, posY+2, posX+4, posY+2, Green);
			LCD_DrawLine(posX+3, posY+3, posX+5, posY+3, Green);
			LCD_DrawLine(posX+2, posY+4, posX+6, posY+4, Green);
			LCD_DrawLine(posX+3, posY+5, posX+5, posY+5, Green);
			LCD_DrawLine(posX+4, posY+6, posX+4, posY+6, Green);
			break;
		case PACMAN:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			LCD_DrawLine(posX+3, posY+1, posX+5, posY+1, Yellow);
			LCD_DrawLine(posX+2, posY+2, posX+6, posY+2, Yellow);
			for (i = 3; i <= 5; i++) {
        LCD_DrawLine(posX+1, posY+i, posX+7, posY+i, Yellow);
			}
			LCD_DrawLine(posX+2, posY+6, posX+6, posY+6, Yellow);
			LCD_DrawLine(posX+3, posY+7, posX+5, posY+7, Yellow);
			break;
		case GHOST:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
				LCD_DrawLine(posX+3, posY+1, posX+5, posY+1, Red);
				LCD_DrawLine(posX+2, posY+2, posX+6, posY+2, Red);
				for (i = 3; i <= 5; i++) {
					LCD_DrawLine(posX+1, posY+i, posX+7, posY+i, Red);
				}
				LCD_DrawLine(posX+2, posY+6, posX+6, posY+6, Red);
				LCD_DrawLine(posX+3, posY+7, posX+5, posY+7, Red);
				break;
		case TELEPORT:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			break;
		case GATE:
			for (i = 1; i <= 7; i++) {
        LCD_DrawLine(posX + 1, posY + i, posX + 7, posY + i, Black);
			}
			for (i = 3; i <= 5; i++) {
        LCD_DrawLine(posX+1, posY + i, posX + 8, posY +i, Magenta);
			}
			break;
		default:
			break;
	}
	
}

void DrawPacmanLife() {

		LCD_DrawBlock(PACMAN, 8*vite+15, 285);
		
}

void ClearPacmanLife() {

		LCD_DrawBlock(NOWALL, 8*vite+15, 285);	
}

void pause() {
	disable_timer(0);
	disable_timer(1);
  GUI_Text(100, 20, (uint8_t *)"PAUSE!", Red, White);
}

void DrawPacmanMap(int newPacmanMap[29][30]) {
	
	pause();
	
	GUI_Text(20, 0, (uint8_t *) "Game over in:", White, Black);
	extern int count;
	char countString[8];
	sprintf(countString, "%d ", count);
	//GUI_Text(20, 20, (uint8_t *) countString, White, Black);
	
	GUI_Text(175, 0, (uint8_t *) "Score:", White, Black);
	//GUI_Text(190, 20, (uint8_t *) "00", White, Black);
	
	
	
    int row = 0;
		for (row = 0; row < 29; row++) {       // Itera su tutte le righe della matrice
        int col = 0;
				for (col = 0; col < 30; col++) {   // Itera su tutte le colonne della matrice
            int blockType = newPacmanMap[row][col];   // Ottieni il tipo di blocco
            uint16_t posX = 0 + (col * 8);   // Calcola la posizione X
            uint16_t posY = 48 + (row * 8);    // Calcola la posizione Y
            LCD_DrawBlock(blockType, posX, posY);               // Disegna il blocco
        }
    }
		
		DrawPacmanLife();
}

void victory() {
	disable_timer(0);
	disable_timer(1);
	LCD_Clear(Black);
  GUI_Text(80, 140, (uint8_t *)"VICTORY!", Red, White);
}

void resume() {
	GUI_Text(100, 20, (uint8_t *)"PAUSE!", Black, Black);
	enable_timer(0);
	enable_timer(1);
}

void gameOver() {
	disable_timer(0);
	disable_timer(1);
	NVIC_DisableIRQ(EINT0_IRQn);
  LCD_Clear(Black);
  GUI_Text(75, 140, (uint8_t *)"GAME OVER!", Blue, White);
}

void generateCasualPills() {
    
    int allPositions[870];  
    int countPerRand = 0;

		int y=0;
		int x=0;
    
    for (y = 0; y < 29; y++) {
        for (x = 0; x < 30; x++) {
            if (newPacmanMap[y][x] == 17) {  
                allPositions[countPerRand] = (y * 30) + x;  
                countPerRand++;  
            }
        }
    }
    
    
    if (countPerRand > 0) {
        
        int randomIndex = rand() % countPerRand;  
        int pos = allPositions[randomIndex];
				int posX = pos % 30; 
        int posY = pos / 30;   
        
        
        newPacmanMap[posY][posX] = 18;  

        
        uint16_t X = OFFSET_LEFT + (posX * BLOCK_SIZE);
        uint16_t Y = OFFSET_TOP + (posY * BLOCK_SIZE);

        
        LCD_DrawBlock(18, X, Y);  
    }
} 

void addPoints(int numOfPoints){
		score = score + numOfPoints;
		if(score % 1000 == 0 && (score / 1000 > lastScore)){
			vite++;
			DrawPacmanLife();
		}
	
		char scoreString[16];
		sprintf(scoreString, "%d", score);
		GUI_Text(190, 20, (uint16_t *) scoreString, White, Black);
		nPills--;
}


void updatePacManPosition(int newPosInMatrix, int op, int x, int y){
	newPacmanMap[posPacManY][posPacManX] = 15;
	int blockType = newPacmanMap[posPacManY][posPacManX];
	uint16_t posX = OFFSET_LEFT + (posPacManX * BLOCK_SIZE);   
	uint16_t posY = OFFSET_TOP + (posPacManY * BLOCK_SIZE);    
	LCD_DrawBlock(blockType, posX, posY);
		
	if(op==0){
		if(y==1){
			posPacManY = posPacManY - newPosInMatrix;
		}else if(x==1){
			posPacManX = posPacManX - newPosInMatrix;
		}
	}else if(op==1){
		if(y==1){
			posPacManY = posPacManY + newPosInMatrix;
		}else if(x==1){
			posPacManX = posPacManX + newPosInMatrix;
		}
	}
	
	newPacmanMap[posPacManY][posPacManX] = 19;
	blockType = newPacmanMap[posPacManY][posPacManX];
	posX = OFFSET_LEFT + (posPacManX * BLOCK_SIZE);   
	posY = OFFSET_TOP + (posPacManY * BLOCK_SIZE);  
	LCD_DrawBlock(blockType, posX, posY);
}

void updateGhostPosition(int op ,int x, int y){
	
	if(op==0 && x==0 && y==1){
		int blockUp = newPacmanMap[posGhostY-y][posGhostX];
		
		int blockType = lastBlock;
		lastBlock = newPacmanMap[posGhostY-y][posGhostX];
		uint16_t posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);  
		uint16_t posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);    
		LCD_DrawBlock(blockType, posX, posY);
		
		posGhostY = posGhostY - y;
		posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);
		posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE); 
		LCD_DrawBlock(23, posX, posY);
	}else if(op==1 && x==0 && y==1){
		int blockDown = newPacmanMap[posGhostY+y][posGhostX];

	
		int blockType = lastBlock;
		lastBlock = newPacmanMap[posGhostY+y][posGhostX];
		uint16_t posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   
		uint16_t posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);  
		LCD_DrawBlock(blockType, posX, posY);
		
		// Aggiorna la posizione di PacMan
		posGhostY = posGhostY + y;
		posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   
		posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);   
		LCD_DrawBlock(23, posX, posY);
	}else if(op==0 && x==1 && y==0){
		int blockType = lastBlock;
		lastBlock = newPacmanMap[posGhostY][posGhostX-x];
		uint16_t posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   // Calcola la posizione X
		uint16_t posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);    // Calcola la posizione Y
		LCD_DrawBlock(blockType, posX, posY);
		
		// Aggiorna la posizione di PacMan
		posGhostX = posGhostX - x;
		posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   // Calcola la posizione X
		posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);    // Calcola la posizione Y
		LCD_DrawBlock(23, posX, posY);
		
	}else if(op==1 && x==1 && y==0){
		int blockType = lastBlock;
		lastBlock = newPacmanMap[posGhostY][posGhostX+x];
		uint16_t posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   // Calcola la posizione X
		uint16_t posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);    // Calcola la posizione Y
		LCD_DrawBlock(blockType, posX, posY);
		
		// Aggiorna la posizione di PacMan
		posGhostX = posGhostX + x;
		posX = OFFSET_LEFT + (posGhostX * BLOCK_SIZE);   // Calcola la posizione X
		posY = OFFSET_TOP + (posGhostY * BLOCK_SIZE);    // Calcola la posizione Y
		LCD_DrawBlock(23, posX, posY);
	}
}

void pacmanAndGhostInSamePosition(){
			move = 0;
			newPacmanMap[posPacManY][posPacManX] = 15;
			LCD_DrawBlock(15, posPacManY, posPacManX);
			posPacManX = 14;
			posPacManY = 9;
			newPacmanMap[posPacManY][posPacManX] = 19;
			uint16_t posX = 0 + (posGhostX * 8);   // Calcola la posizione X
			uint16_t posY = 48 + (posGhostY * 8);    // Calcola la posizione Y
			LCD_DrawBlock(23, posX, posY);
			uint16_t posXP = 0 + (posPacManX * 8);   // Calcola la posizione X
			uint16_t posYP = 48 + (posPacManY * 8);    // Calcola la posizione Y
			LCD_DrawBlock(19, posXP, posYP);
			ClearPacmanLife();
			vite--;
			viteCAN--;
			DrawPacmanLife();
}

void ghostSpeed(int count) {
    if (count <= 60 && count > 40) {
        ghostMoveCounter = 4; 
    }
    if (count <= 40 && count > 20) {
        ghostMoveCounter = 3;
    }
    if (count <= 20 && count > 10) {
        ghostMoveCounter = 2;
    }
}

void algorithmGhost() {
    int bestDirection = -1;   
    int minDistanceToPacMan = 10000;  
    int maxDistanceToPacMan = -1;  

    
    int currentDirection = 1;

    for (currentDirection = 1; currentDirection <= 4; currentDirection++) {
        int ghostX = posGhostX;
        int ghostY = posGhostY;

        
        switch (currentDirection) {
            case 1:  
                ghostY = posGhostY - 1;
                if (newPacmanMap[ghostY][ghostX] == 21) {
                    moveGhostUp(1);
                    break;
                }
                break;
            case 2:  
                ghostY = posGhostY + 1;
                if (newPacmanMap[ghostY][ghostX] == 21) {
                    continue; 
                }
                break;
            case 3:  
                ghostX = posGhostX - 1;
                break;
            case 4: 
                ghostX = posGhostX + 1;
                break;
        }

        
        if (ghostX >= 0 && ghostX < 30 && ghostY >= 0 && ghostY < 25 &&
            newPacmanMap[ghostY][ghostX] != 16) {

            
            if (ghostX == lastGhostPosX && ghostY == lastGhostPosY) {
                continue;  
            }

            
            int distanceToPacMan = abs(ghostX - posPacManX) + abs(ghostY - posPacManY);

            
            if (distanceToPacMan < minDistanceToPacMan) {
                minDistanceToPacMan = distanceToPacMan;
                bestDirection = currentDirection;
            }
        }
    }

    
    lastGhostPosX = posGhostX;
    lastGhostPosY = posGhostY;

    
    if (bestDirection != -1) {
        switch (bestDirection) {
            case 1:  
                moveGhostUp();
                break;
            case 2:  
                moveGhostDown();
                break;
            case 3:  
                moveGhostLeft();
                break;
            case 4:  
                moveGhostRight();
                break;
        }
    }
		
		ghostSpeed(count);
}


void generateCasualValuesTime() {
		int i=0;
		int j=0;

    for (i = 0; i < 6; i++) {
        int newPill;
        bool isUnique;

        do {
            newPill = rand() % 60; 
            isUnique = true;

            
            for (j = 0; j < i; j++) {
                if (causualValuesTime[j] == newPill) {
                    isUnique = false;
                    break;
                }
            }
        } while (!isUnique); 

        causualValuesTime[i] = newPill;
    }
}

void pacmanMovement(){
	if (gameStatus == -1) return;
	
	if (vite == 0 && gameStatus != -1){
		gameStatus = -1;
		move = 0;
		gameOver();
		return;
	}
	
	if (count == 0 && gameStatus != -1) {
		gameStatus = -1;
		move = 0;
		gameOver();
		return;
	}
	
	if (nPills == 0 && gameStatus != -1) {
		gameStatus = -1;
		victory();
		return;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){					
		move = 1;
		gameStatus = 0;
	} else if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		move = 2;
		gameStatus = 0;
	}	else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		move = 3;
		gameStatus = 0;
	}	else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		move = 4;
		gameStatus = 0;
	}
	
	switch (move) {
		case 0:
			moveStop();
			break;
		case 1:
			moveUp();
			break;
		case 2:
			moveRight();
			break;
		case 3:
			moveLeft();
			break;
		case 4:
			moveDown();
			break;
		default:
			break;
	}
	
		//se il fantasma tocca pacman
	if (posPacManX == posGhostX && posPacManY == posGhostY){
		pacmanAndGhostInSamePosition();
	}
	
	if(ghostMoveCounter == 0 && gameStatus == 0){
		algorithmGhost();
	}
	
	if(gameStatus == 0 && ghostMoveCounter > 0){
		ghostMoveCounter--;
	}
	
	if(count == 40){
		ghostMoveCounter = 3;
	}
	
	if(count == 20){
		ghostMoveCounter = 2;
	}
}

