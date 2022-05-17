
#pragma once
#define TILE_PLACE_SUCCESS 1
#define TILE_PLACE_FAILURE 0

typedef enum {
	TILE_PLAYER_OWNED,
	TILE_COMPUTER_OWNED,
	TILE_NOT_OWNED
} TileValue;

typedef struct {
	TileValue TopLeft;
	TileValue TopMiddle;
	TileValue TopRight;
	TileValue MiddleLeft;
	TileValue MiddleMiddle;
	TileValue MiddleRight;
	TileValue BottomLeft;
	TileValue BottomMiddle;
	TileValue BottomRight;

	enum {
		GAME_RUNNING,
		GAME_NOT_RUNNING
	} State;

	enum {
		RESULT_PLAYER_WON,
		RESULT_COMPUTER_WON,
		RESULT_DRAW,
		RESULT_NONE
	} Result;
} Game;

Game* InitGame();
void DrawTable(Game* game);
int ProcessUserInput(Game* game, char* input);
void ProcessComputerGuess(Game* game);
void UpdateGame(Game* game);
