#pragma once

#include "Game/Play/Player.h"
#include "Game/Play/Item/Item.h"
#include "PlatformTypes.h"
#include <stdbool.h>

extern const uint16_t PalNumTableNormalBlocks[11];
extern const uint16_t PalNumTableItemBlocks[NUMITEMTYPES];
extern const Color* PalTableItemFieldBorder[NUMITEMTYPES];

void SetFieldBorderColor(Player* player, ItemType itemType);

typedef enum ShowBlockType {
	SHOWBLOCKTYPE_ACTIVE,
	SHOWBLOCKTYPE_NEXT,
	SHOWBLOCKTYPE_TLS
} ShowBlockType;
void ShowBlock(Player* player, ShowBlockType showBlockType, bool show);

void ShowField(Player* player);

void ShowFieldPlus(Player* player);
