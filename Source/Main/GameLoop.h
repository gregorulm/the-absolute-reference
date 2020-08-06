#pragma once

#include "Main/MainLoop.h"
#include "Video/VideoDefs.h"
#include "PlatformTypes.h"
#include <stdint.h>

extern ROMDATA Color PalCycleTextPal0[NUMPALCOLORS_4BPP];

MainLoopState RunGameLoop();
uint8_t NumVersusRoundsSetting();