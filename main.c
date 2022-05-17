
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#define INPUT_BUFFER_LEN 255

char* ReadUserInput() {
	printf(">>> ");

	char buffer[INPUT_BUFFER_LEN];
	gets_s(buffer, INPUT_BUFFER_LEN);
	return buffer;
}

int main() {
	Game* game = InitGame();
	DrawTable(game);

	while (game->State == GAME_RUNNING) {
		while (1) {
			char* input = ReadUserInput();

			if (!ProcessUserInput(game, input)) {
				printf("[-] Unable to place the specified tile here\n");
			}
			else {
				UpdateGame(game);
				break;
			}
		}
		
		ProcessComputerGuess(game);
		UpdateGame(game);
		DrawTable(game);
	}

	if (game->Result == RESULT_PLAYER_WON) {
		printf("Player has won the game!\n");
	}
	else if (game->Result == RESULT_COMPUTER_WON) {
		printf("Computer has won the game!\n");
	}
	else if (game->Result == RESULT_DRAW) {
		printf("Game ended in draw!\n");
	}

	free(game);
}





