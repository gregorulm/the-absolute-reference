#pragma once

#include "Input/InputTypes.h"
#include <stdint.h>

typedef enum StartPlayerFlag {
	STARTPLAYER_NONE = 0,
	STARTPLAYER_1P = 1 << PLAYER1,
	STARTPLAYER_2P = 1 << PLAYER2
} StartPlayerFlag;
extern StartPlayerFlag StartPlayers;

typedef enum MainLoopState {
	MAINLOOP_ATTRACT,
	MAINLOOP_GAME,
	MAINLOOP_TEST
} MainLoopState;
extern MainLoopState MainLoop;

void RunMainLoop();
