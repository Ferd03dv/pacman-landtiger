#include <stdint.h>
/* Private function prototypes -----------------------------------------------*/
void LCD_DrawBlock(uint8_t blockType, uint16_t posX, uint16_t posY);
void DrawPacmanLife();
void ClearPacmanLife();
void DrawPacmanMap(int newPacmanMap[29][30]);
void victory();
void pause();
void resume();
void gameOver();
void showPill();
void addPoints(int numOfPoints);
void updatePacManPosition(int newPosInMatrix, int op, int x, int y);
void updateGhostPosition(int op, int x, int y);
void pacmanMovement();


