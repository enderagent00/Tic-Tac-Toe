
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "ansi-colours.h"
#include "game.h"
#define TOP_LEFT "TL"
#define TOP_MIDDLE "TM"
#define TOP_RIGHT "TR"
#define MIDDLE_LEFT "ML"
#define MIDDLE_MIDDLE "MM"
#define MIDDLE_RIGHT "MR"
#define BOTTOM_LEFT "BL"
#define BOTTOM_MIDDLE "BM"
#define BOTTOM_RIGHT "BR"
#define NAUGHT "0"
#define CROSS "X"
#define EMPTY " "
#define OUTPUT_BUFFER_LEN 1024
#define WINNER_DETECTED 1
#define WINNER_NOT_DETECTED 0

/* 
	Returns the output string of a tile's value 
*/
static const char* TileValueToString(TileValue value) {
	switch (value) {
		case TILE_PLAYER_OWNED:
			return NAUGHT;
		case TILE_COMPUTER_OWNED:
			return CROSS;
		case TILE_NOT_OWNED:
			return EMPTY;
	}
}

/*
	Returns the output string of a tile with ANSI colours
		- Green = Owned by the player
		- Red = Owned by the computer
		- White = Not owned by either players
*/
static char* TileValueApplyColour(Game* game, int* tilevalue) {
	const char* tilestr = TileValueToString(*tilevalue);

	if (*tilevalue == TILE_PLAYER_OWNED) {
		static char output[32];
		sprintf_s(output, 32, "%s%s%s", ANSI_COLOUR_GREEN, tilestr, ANSI_COLOUR_RESET);
		return output;
	}
	else if (*tilevalue == TILE_COMPUTER_OWNED) {
		static char output[32];
		sprintf_s(output, 32, "%s%s%s", ANSI_COLOUR_RED, tilestr, ANSI_COLOUR_RESET);
		return output;
	}

	return tilestr;
}

/*
	Returns a random tile 
*/
static int* GetRandomTile(Game* game) {
	int seed = time(0) / 2;
	srand(seed);
	int randint = rand() % 9 + 1;

	switch (randint) {
		case 1:
			return &game->TopLeft;
		case 2:
			return &game->TopMiddle;
		case 3:
			return &game->TopRight;
		case 4:
			return &game->MiddleLeft;
		case 5:
			return &game->MiddleMiddle;
		case 6:
			return &game->MiddleRight;
		case 7:
			return &game->BottomLeft;
		case 8:
			return &game->BottomMiddle;
		case 9:
			return &game->BottomRight;
	}
}

/*
	Detect a winner
*/
static int DetectWin(Game* game, int tileState) {
	// Detect horizontal win

	if (game->TopLeft == tileState && game->TopMiddle == tileState && game->TopRight == tileState) {
		return WINNER_DETECTED;
	}
	if (game->MiddleLeft == tileState && game->MiddleMiddle == tileState && game->MiddleRight == tileState) {
		return WINNER_DETECTED;
	}
	if (game->BottomLeft == tileState && game->BottomMiddle == tileState && game->BottomRight == tileState) {
		return WINNER_DETECTED;
	}

	// Detect vertical win

	if (game->TopLeft == tileState && game->MiddleLeft == tileState && game->BottomLeft == tileState) {
		return WINNER_DETECTED;
	}
	if (game->TopMiddle == tileState && game->MiddleMiddle == tileState && game->BottomMiddle == tileState) {
		return WINNER_DETECTED;
	}	
	if (game->TopRight == tileState && game->MiddleRight == tileState && game->BottomRight == tileState) {
		return WINNER_DETECTED;
	}

	// Detect diagonal win

	if (game->TopLeft == tileState && game->MiddleMiddle == tileState && game->BottomRight == tileState) {
		return WINNER_DETECTED;
	}
	else if (game->BottomLeft == tileState && game->MiddleMiddle == tileState && game->TopRight == tileState) {
		return WINNER_DETECTED;
	}

	return WINNER_NOT_DETECTED;
}

/*
	Initialize the game on the heap
*/
Game* InitGame() {
	Game* game = calloc(1, sizeof(Game));
	game->TopLeft = TILE_NOT_OWNED;
	game->TopMiddle = TILE_NOT_OWNED;
	game->TopRight = TILE_NOT_OWNED;
	game->MiddleLeft = TILE_NOT_OWNED;
	game->MiddleMiddle = TILE_NOT_OWNED;
	game->MiddleRight = TILE_NOT_OWNED;
	game->BottomLeft = TILE_NOT_OWNED;
	game->BottomMiddle = TILE_NOT_OWNED;
	game->BottomRight = TILE_NOT_OWNED;
	game->State = GAME_RUNNING;
	game->Result = RESULT_NONE;

	return game;
}

/*
	Draws the table to the console
*/
void DrawTable(Game* game) {
	const char* format = "\n\
\t      |      |\n\
\t  %s   |  %s   |   %s\n\
\t      |      |\n\
\t--------------------\n\
\t      |      |\n\
\t  %s   |  %s   |   %s\n\
\t      |      |\n\
\t--------------------\n\
\t      |      |\n\
\t  %s   |  %s   |  %s\n\
\t      |      |\n\n\n";

	char* topLeft = TileValueApplyColour(game, &game->TopLeft);
	char* topMiddle = TileValueApplyColour(game, &game->TopMiddle);
	char* topRight = TileValueApplyColour(game, &game->TopRight);
	char* middleLeft = TileValueApplyColour(game, &game->MiddleLeft);
	char* middleMiddle = TileValueApplyColour(game, &game->MiddleMiddle);
	char* middleRight = TileValueApplyColour(game, &game->MiddleRight);
	char* bottomLeft = TileValueApplyColour(game, &game->BottomLeft);
	char* bottomMiddle = TileValueApplyColour(game, &game->BottomMiddle);
	char* bottomRight = TileValueApplyColour(game, &game->BottomRight);

	char output[OUTPUT_BUFFER_LEN];

	sprintf_s(output, OUTPUT_BUFFER_LEN, format,
		topLeft, topMiddle, topRight,
		middleLeft, middleMiddle, middleRight,
		bottomLeft, bottomMiddle, bottomRight
	);

	system("cls");
	printf(output);
}

/*
	Processes a user's input
*/
int ProcessUserInput(Game* game, char* input) {
	if (strcmp(input, TOP_LEFT) == 0) {
		if (game->TopLeft == TILE_NOT_OWNED) {
			game->TopLeft = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, TOP_MIDDLE) == 0) {
		if (game->TopMiddle == TILE_NOT_OWNED) {
			game->TopMiddle = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, TOP_RIGHT) == 0) {
		if (game->TopRight == TILE_NOT_OWNED) {
			game->TopRight = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, MIDDLE_LEFT) == 0) {
		if (game->MiddleLeft == TILE_NOT_OWNED) {
			game->MiddleLeft = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, MIDDLE_MIDDLE) == 0) {
		if (game->MiddleMiddle == TILE_NOT_OWNED) {
			game->MiddleMiddle = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, MIDDLE_RIGHT) == 0) {
		if (game->MiddleRight == TILE_NOT_OWNED) {
			game->MiddleRight = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, BOTTOM_LEFT) == 0) {
		if (game->BottomLeft == TILE_NOT_OWNED) {
			game->BottomLeft = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, BOTTOM_MIDDLE) == 0) {
		if (game->BottomMiddle == TILE_NOT_OWNED) {
			game->BottomMiddle = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}
	else if (strcmp(input, BOTTOM_RIGHT) == 0) {
		if (game->BottomRight == TILE_NOT_OWNED) {
			game->BottomRight = TILE_PLAYER_OWNED;
			return TILE_PLACE_SUCCESS;
		}
	}

	return TILE_PLACE_FAILURE;
}

/*
	Allows the computer to randomly select a tile
*/
void ProcessComputerGuess(Game* game) {
	if (game->State == GAME_NOT_RUNNING) {
		return;
	}

	while (1) {
		int* valptr = GetRandomTile(game);

		if (*valptr == TILE_NOT_OWNED) {
			*valptr = TILE_COMPUTER_OWNED;
			break;
		}
		else {
			Sleep(100);
		}
	}
}

/*
	Updates the game's information based on what has happened since
	the last update
*/
void UpdateGame(Game* game) {
	if (game->State == GAME_NOT_RUNNING) {
		return;
	}

	if (game->TopLeft != TILE_NOT_OWNED && game->TopMiddle != TILE_NOT_OWNED && game->TopRight != TILE_NOT_OWNED &&
		game->MiddleLeft != TILE_NOT_OWNED && game->MiddleMiddle != TILE_NOT_OWNED && game->MiddleRight != TILE_NOT_OWNED &&
		game->BottomLeft != TILE_NOT_OWNED && game->BottomMiddle != TILE_NOT_OWNED && game->BottomRight) {

		game->State = GAME_NOT_RUNNING;
		game->Result = RESULT_DRAW;
	}
	else if (DetectWin(game, TILE_PLAYER_OWNED)) {
		game->State = GAME_NOT_RUNNING;
		game->Result = RESULT_PLAYER_WON;
	}
	else if (DetectWin(game, TILE_COMPUTER_OWNED)) {
		game->State = GAME_NOT_RUNNING;
		game->Result = RESULT_COMPUTER_WON;
	}
}


