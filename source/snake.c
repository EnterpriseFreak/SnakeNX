#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STATE_TITLE 0
#define STATE_GMAIN 1
#define STATE_GOVER 2
#define STATE_PAUSE 3
#define STATE_BREAK 5

typedef struct{
	int x;
	int y;
}pos2D;

u8 state = STATE_TITLE;
bool clearScreen = true;

pos2D food;
pos2D head;
pos2D body[80*45];
u16 lenght;
u8 direction;

void gameover()
{
	if (clearScreen) { consoleClear(); clearScreen = false; };
	
	hidScanInput();
	u64 keys = hidKeysDown(CONTROLLER_P1_AUTO);
	
	if (keys & KEY_MINUS) state = STATE_BREAK;
	
	if (keys & KEY_PLUS)
	{
		clearScreen = true;
		state = STATE_TITLE;
	}
	
	printf("\x1b[22;36HGAMEOVER");
}

void pause()
{
	if (clearScreen) { consoleClear(); clearScreen = false; };
	
	hidScanInput();
	u64 keys = hidKeysDown(CONTROLLER_P1_AUTO);
	
	if (keys & KEY_MINUS) state = STATE_BREAK;
	
	if (keys & KEY_PLUS)
	{
		clearScreen = true;
		state = STATE_GMAIN;
	}
	
	printf("\x1b[22;37HPAUSE!");
}

void gamemain()
{
	if (clearScreen) { consoleClear(); clearScreen = false; };
	
	printf("\x1b[%d;%dH ", body[0].y, body[0].x);
	
	hidScanInput();
	u64 keys = hidKeysDown(CONTROLLER_P1_AUTO);
	
	//DPAD
	if (keys & KEY_DUP && direction != 3) direction = 2;
	if (keys & KEY_DDOWN && direction != 2) direction = 3;
	if (keys & KEY_DRIGHT && direction != 0) direction = 1;
	if (keys & KEY_DLEFT &&direction != 1) direction = 0;
	
	//STICK
	if (keys & KEY_LSTICK_UP && direction != 3) direction = 2;
	if (keys & KEY_LSTICK_DOWN && direction != 2) direction = 3;
	if (keys & KEY_LSTICK_RIGHT && direction != 0) direction = 1;
	if (keys & KEY_LSTICK_LEFT &&direction != 1) direction = 0;
	
	
	for (int moveTail = 0; moveTail < lenght; moveTail++)
	{
		body[moveTail] = body[moveTail+1];
	}
	body[lenght-1] = head;
	
	if (direction == 0) head.x--;
	if (direction == 1) head.x++;
	if (direction == 2) head.y--;
	if (direction == 3) head.y++;
	
	if (head.x < 0 || head.x > 79 || head.y < 0 || head.y > 44)
	{
		clearScreen = true;
		state = STATE_GOVER;
	}
	
	for (int bited = 0; bited < lenght; bited++)
	{
		if (head.x == body[bited].x && head.y == body[bited].y)
		{
			clearScreen = true;
			state = STATE_GOVER;
		}
	}
	
	if (head.x == food.x && head.y == food.y)
	{
		
		lenght++;
		food.y = rand() % 44;
		food.x = rand() % 79;
	}
	
	if (keys & KEY_MINUS) state = STATE_BREAK;
	
	if (keys & KEY_PLUS)
	{
		clearScreen = true;
		state = STATE_PAUSE;
	}
	
	for (int drawTail = 0; drawTail < lenght; drawTail++)
	{
		printf("\x1b[%d;%dH=", body[drawTail].y, body[drawTail].x);
	}
	
	printf("\x1b[%d;%dH<", head.y, head.x);
	printf("\x1b[%d;%dH@", food.y, food.x);
}

void titlescreen()
{
	if (clearScreen) { consoleClear(); clearScreen = false; };
	
	hidScanInput();
	u64 keys = hidKeysDown(CONTROLLER_P1_AUTO);
	
	if (keys & KEY_MINUS) state = STATE_BREAK;
	
	if (keys & KEY_PLUS)
	{
		food.x = rand() % 79;
		food.y = rand() % 44;
		
		head.x = 41;
		head.y = 22;
		
		body[2].x = 40;
		body[2].y = 22;
		body[1].x = 39;
		body[1].y = 22;
		body[0].x = 38;
		body[0].y = 22;
		
		lenght = 3;
		direction = 1;
		
		clearScreen = true;
		state = STATE_GMAIN;
	};
	
	printf("\x1b[22;27HPRESS PLUS TO PLAY SNAKE !");
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	consoleInit(NULL);
	
	while(appletMainLoop())
	{
		if (state == STATE_TITLE) { titlescreen(); };
		if (state == STATE_GMAIN) { gamemain(); };
		if (state == STATE_PAUSE) { pause(); };
		if (state == STATE_GOVER) { gameover(); };
		if (state == STATE_BREAK) { break; };
		
		consoleUpdate(NULL);
        consoleUpdate(NULL);
        consoleUpdate(NULL);
        consoleUpdate(NULL);
	}
	
	consoleExit(NULL);
	return 0;
}
