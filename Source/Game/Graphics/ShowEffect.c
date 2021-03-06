#include "Game/Graphics/ShowEffect.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/DisplayObject.h"
#include "Game/Play/Block.h"
#include "Video/Video.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "Main/Frame.h"
#include "Lib/Math.h"
#include <assert.h>

typedef struct BlockExplosionData {
	const ObjectData* objectTable;
	uint16_t palNum;
	int16_t y;
	int16_t x;
} BlockExplosionData;

static const ObjectData* ObjectTablesBlockExplosions[NUMBLOCKEXPLOSIONTYPES] = {
	// TODO: Give these names based on their colors. They don't actually
	// correspond to block colors, because they're randomly selected.
	&OBJECTTABLES_BLOCKEXPLOSIONS[0 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[1 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[2 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[3 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[4 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[5 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[6 * NUMBLOCKEXPLOSIONS],
	&OBJECTTABLES_BLOCKEXPLOSIONS[7 * NUMBLOCKEXPLOSIONS]
};

#define frames values[0]

static void UpdateEntityFieldBlockExplosion(Entity* entity);

void ShowFieldBlockExplosion(Player* player, int16_t row, int16_t col) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityFieldBlockExplosion;
		entity->frames = 0;

		entity->data.unionData.player = player;

		ENTITY_INST_DATA_PTR(BlockExplosionData, data, entity);
		data->x = player->screenPos[0] + 8 * col - 8 * (player->matrixWidth / 2);
		data->y = player->screenPos[1] - 8 * row - 6;
		data->objectTable = ObjectTablesBlockExplosions[Rand(8u) % 8];
		uint8_t blockNum = TOBLOCKNUM(MATRIX(player, row, col).block & BLOCK_TYPE);
		// BUG: The SHOTGUN! item calls this function after setting matrix
		// blocks to empty, resulting in this function reading BLOCKTYPE_EMPTY
		// from the matrix, which then results in an out-of-bounds access of
		// PalNumTableNormalBlocks. It ends up using palette 10 in that case,
		// reading that number from somewhere in memory, and this fix
		// reproduces that behavior.
		if ((MATRIX(player, row, col).block & BLOCK_TYPE) == BLOCKTYPE_EMPTY) {
			data->palNum = 10u;
		}
		else {
			assert(blockNum < lengthof(PalNumTableNormalBlocks));
			data->palNum = PalNumTableNormalBlocks[blockNum];
		}
	}
}

static void UpdateEntityFieldBlockExplosion(Entity* entity) {
	STATIC_ASSERT(sizeof(BlockExplosionData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BlockExplosionData, data, entity);
	DisplayObject(&data->objectTable[entity->frames], data->y, data->x, data->palNum + 9u, 115u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frames >= 32) {
		FreeEntity(entity);
	}
}

#undef frames

const uint8_t PalNumTableFireworks[5] = { 203u, 204u, 205u, 206u, 207u };

#define frames values[0]

static void UpdateEntityFireworks(Entity* entity);

void ShowFireworks(Player* player, int16_t row, int16_t col, uint32_t seed) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityFireworks;
		entity->data.unionData.player = player;
		entity->frames = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = player->screenPos[0] + (col + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->screenPos[1] - row * 8 + 10;
		data->objectTable = OBJECTTABLE_FIREWORKS;
		data->palNum = PalNumTableFireworks[Rand(5u)];
	}
}

static void UpdateEntityFireworks(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	data->palNum = PalNumTableFireworks[Rand(5u)];
	int16_t scaleOffset = ((entity->frames + UNSCALED) * 32) / UNSCALED;
	DisplayObjectEx(
		&data->objectTable[entity->frames],
		data->y - scaleOffset,
		data->x - scaleOffset,
		(uint8_t)data->palNum,
		115u,
		entity->frames + UNSCALED,
		entity->frames + UNSCALED,
		false
	);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frames >= 32) {
		FreeEntity(entity);
	}
}

#undef frames

#define frames values[0]

static void UpdateEntityAllClear(Entity* entity);

void ShowAllClear(Player* player, int16_t row, int16_t col, bool unused) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityAllClear;
		entity->data.unionData.player = player;
		entity->frames = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = player->screenPos[0] + (col + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->screenPos[1] - row * 8 + 10;
		data->objectTable = OBJECTTABLE_FIREWORKS;
		data->palNum = PalNumTableFireworks[Rand(5u)];
	}
}

static void UpdateEntityAllClear(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	Player* player = entity->data.unionData.player;
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	data->palNum = PalNumTableFireworks[Rand(5u)];
	if (entity->frames < 32) {
		DisplayObject(&data->objectTable[entity->frames], data->y - 32, data->x - 32, (uint8_t)data->palNum, 115u);
	}

	uint8_t palNum = ScreenTime % 4 == 0u ? 2u : 8u;
	DisplayObject(OBJECT_BRAVO, 100, player->screenPos[0] - 48, palNum, 115u);
	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frames >= 60) {
		FreeEntity(entity);
	}
}

#undef frames

typedef struct {
	const ObjectData* objectTables[2];
	uint16_t palNum;
	int16_t screenPositions[2][2];
} structUNK_60174F8;

#define frames values[0]

static void UNK_60174F8(Entity* entity);

void UNK_60173B4(Player* player, int16_t row, int16_t col) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UNK_60174F8;
		entity->data.unionData.player = player;
		entity->frames = 0;

		ENTITY_INST_DATA_PTR(structUNK_60174F8, data, entity);

		data->screenPositions[1][0] = player->screenPos[0] + 8 * (col + 2) - 8 * (player->matrixWidth / 2);
		data->screenPositions[0][0] = player->screenPos[1] - 8 * row + 10;

		data->screenPositions[1][1] = data->screenPositions[1][0] + Rand(10u) - 5;
		data->screenPositions[0][1] = data->screenPositions[0][0] + Rand(10u) - 5;

		const ObjectData** objectTable = data->objectTables;
		for (size_t i = 0; i < 2; i++) {
			*objectTable = ObjectTablesBlockExplosions[Rand(8u) % 8];
			const uint8_t blockNum = TOBLOCKNUM(player->activeBlock & BLOCK_TYPE);
			assert(blockNum < lengthof(PalNumTableNormalBlocks));
			data->palNum = PalNumTableNormalBlocks[blockNum];
		}
	}
}

static void UNK_60174F8(Entity* entity) {
	STATIC_ASSERT(sizeof(structUNK_60174F8) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(structUNK_60174F8, data, entity);

	const ObjectData** objectTable = data->objectTables;
	for (size_t i = 0; i < 2; i++, objectTable++) {
		DisplayObject(objectTable[entity->frames], data->screenPositions[0][i], data->screenPositions[1][i], data->palNum + 9u, 115u);
	}

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frames >= 32) {
		FreeEntity(entity);
	}
}

#undef frames

typedef struct SingleClearData {
	const ObjectData* objectTables[FIELD_DOUBLESWIDTH];
	uint16_t palNums[FIELD_DOUBLESWIDTH];
	int16_t y;
} SingleClearData;

#define frames values[0]
#define explosionsWidth values[1]
#define clearRow values[3]

static void UpdateEntitySingleClear(Entity* entity);

void ShowLineClear(Player* player, int16_t row) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntitySingleClear;
		entity->data.unionData.player = player;
		entity->frames = 0;
		entity->explosionsWidth = player->matrixWidth;
		entity->clearRow = row;

		ENTITY_INST_DATA_PTR(SingleClearData, data, entity);
		data->y = player->screenPos[1] - entity->clearRow * 8 - 6;
		uint32_t explosionSeed = Rand(1999u);
		for (int16_t col = 1; col < entity->explosionsWidth - 1; col++, explosionSeed += 3u) {
			if (player->modeFlags & MODE_BIG) {
				if (row % 3 == col % 3) {
					data->objectTables[col - 1] = ObjectTablesBlockExplosions[explosionSeed % 8];
					const uint8_t blockNum = TOBLOCKNUM(MATRIX(player, row, col).block & BLOCK_TYPE);
					assert(blockNum < lengthof(PalNumTableNormalBlocks));
					data->palNums[col - 1] = PalNumTableNormalBlocks[blockNum];
				}
				else {
					data->objectTables[col - 1] = NULL;
				}
			}
			else {
				if (row % 2 == col % 2) {
					data->objectTables[col - 1] = ObjectTablesBlockExplosions[explosionSeed % 8];
					const uint8_t blockNum = TOBLOCKNUM(MATRIX(player, row, col).block & BLOCK_TYPE);
					assert(blockNum < lengthof(PalNumTableNormalBlocks));
					data->palNums[col - 1] = PalNumTableNormalBlocks[blockNum];
				}
				else {
					data->objectTables[col - 1] = NULL;
				}
			}
		}
	}
}

void ShowStaffClear(Player* player, int16_t row) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntitySingleClear;
		entity->data.unionData.player = player;
		entity->frames = 0;
		entity->explosionsWidth = player->matrixWidth;
		entity->clearRow = row;

		ENTITY_INST_DATA_PTR(SingleClearData, data, entity);
		data->y = player->screenPos[1] - entity->clearRow * 8 - 6;
		uint32_t explosionSeed = Rand(1999u);
		for (int16_t col = 1, odd = 0; col < entity->explosionsWidth - 1; col++, explosionSeed += 3u) {
			if (MATRIX(player, row, col).block != NULLBLOCK) {
				odd = (odd + 1) % 2;
			}

			if (odd) {
				data->objectTables[col - 1] = ObjectTablesBlockExplosions[explosionSeed % 8];

				// BUG: When the matrix block type is empty, the original code
				// would end up accessing the uint16_t located four bytes
				// behind where PalNumTableNormalBlocks is in memory. The code
				// would also access the uint16_t two bytes behind, if the
				// block type is a wall, though that case doesn't occur. The
				// original behavior is reimplemented here, without any
				// undefined behavior.
				uint16_t invalidPalNums[2] = { 0x000Au, 0x785Cu };
				const size_t blockType = MATRIX(player, row, col).block & BLOCK_TYPE;
				if (blockType < BLOCKTYPE_I) {
					data->palNums[col - 1] = *(&invalidPalNums[2] + (int16_t)blockType - 2);
				}
				else {
					assert(TOBLOCKNUM(blockType) < lengthof(PalNumTableNormalBlocks));
					data->palNums[col - 1] = PalNumTableNormalBlocks[TOBLOCKNUM(blockType)];
				}
			}
			else {
				data->objectTables[col - 1] = NULL;
			}
		}
	}
}

static void UpdateEntitySingleClear(Entity* entity) {
	STATIC_ASSERT(sizeof(SingleClearData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(SingleClearData, data, entity);
	int16_t y = data->y;
	int16_t x = entity->data.unionData.player->screenPos[0] - 40;
	// BUG: Should be "col < entity->data.info.player->fieldWidth". Results in
	// line clear explosions spanning only the left 10 columns of a doubles
	// field.
	for (int16_t col = 0; col < FIELD_SINGLEWIDTH; col++, x += 8) {
		if (data->objectTables[col] != NULL) {
			DisplayObject(&data->objectTables[col][entity->frames], y, x, data->palNums[col] + 9u, 125u);
		}
	}

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frames >= 32) {
		FreeEntity(entity);
	}
}

#undef frames
#undef explosionsWidth
#undef clearRow

typedef struct ThrownOutActiveBlockData {
	int16_t y;
	int16_t x;
	Block activeBlock;
	int16_t spread;
	Rotation rotation;
} ThrownOutActiveBlockData;

#define frames values[0]
#define blockVerticalV values[1]

static void UpdateEntityThrownOutActiveBlock(Entity* entity);
static void DisplayThrownOutActiveBlock(Player* player, int16_t x, int16_t y, int16_t spread, Block block, Rotation rotation);

void ShowThrownOutActiveBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityThrownOutActiveBlock;
		entity->data.unionData.player = player;
		entity->frames = 1;
		entity->blockVerticalV = -4;
		ENTITY_INST_DATA_PTR(ThrownOutActiveBlockData, data, entity);
		data->x =
			player->screenPos[0] + player->screenOffset[0] +
			player->activePos[0].integer * 8 - (player->matrixWidth / 2) * 8;
		data->y =
			player->screenPos[1] + player->screenOffset[1] +
			(player->matrixHeight - player->activePos[1].integer - 2) * 8 - (player->matrixHeight - 2) * 8 -
			6;
		data->activeBlock = player->activeBlock;
		data->rotation = player->activeRotation;
		data->spread = 8;
	}
}

static void UpdateEntityThrownOutActiveBlock(Entity* entity) {
	STATIC_ASSERT(sizeof(ThrownOutActiveBlockData) <= sizeoffield(Entity, buffer));

	Player* player = entity->data.unionData.player;
	ENTITY_INST_DATA_PTR(ThrownOutActiveBlockData, data, entity);

	data->y += entity->blockVerticalV;

	int16_t x;
	if (player->num == PLAYER1) {
		x = data->x - entity->frames;
	}
	else {
		x = data->x + entity->frames;
	}

	if (entity->frames % 5 == 0) {
		data->spread++;
	}

	DisplayThrownOutActiveBlock(player, x, data->y, data->spread, data->activeBlock, data->rotation);

	if (CurrentPauseMode < PAUSEMODE_GAME) {
		if (++entity->frames % 2 != 0) {
			entity->blockVerticalV++;
		}

		if (data->y > VIDEO_WIDTH || entity->frames > TIME(0, 2, 0)) {
			FreeEntity(entity);
		}
	}
}

#undef frames
#undef blockVerticalV

static void DisplayThrownOutActiveBlock(Player* player, int16_t x, int16_t y, int16_t spread, Block activeBlock, Rotation rotation) {
	int16_t squareY = y, squareX = x;
	if (activeBlock & BLOCK_BIG) {
		squareY++;
		squareX -= 2;
	}

	// Select the brightness of blocks first.
	int16_t palNum = player->lockFrames % 6;
	if (player->lockFrames % 6 > 0) {
		palNum++;
	}
	if (palNum > 0) {
		if (palNum >= 4) {
			palNum = 4;
		}
	}
	else {
		palNum = 0;
	}
	palNum += 4;

	const ObjectData* blockObject;
	if (activeBlock & BLOCK_HARD) {
		blockObject = OBJECT_HARDBLOCK;
	}
	else if (activeBlock & BLOCK_ITEM) {
		blockObject = &OBJECTTABLE_ITEMBLOCKS[player->activeBlockItemType - 1];
	}
	else {
		blockObject = &OBJECTTABLE_NORMALBLOCKS[0];
	}

	// Select the starting palette number, adding it to the brightness selected
	// earlier.
	uint16_t startPalNum;
	if (activeBlock & BLOCK_ITEM) {
		startPalNum = PalNumTableNormalBlocks[player->activeBlockItemType - 1];
	}
	else if (activeBlock & BLOCK_ROLLROLL) {
		startPalNum = 48u;
	}
	else if (activeBlock & BLOCK_HARD) {
		startPalNum = 128u;
	}
	else if (activeBlock & BLOCK_TRANSFORM) {
		startPalNum = PalNumTableNormalBlocks[Rand(7u)];
	}
	else {
		const uint8_t blockNum = TOBLOCKNUM(activeBlock & BLOCK_TYPE);
		assert(blockNum < lengthof(PalNumTableNormalBlocks));
		startPalNum = PalNumTableNormalBlocks[blockNum];
	}
	palNum += startPalNum;

	int16_t size;
	SpriteScale scale;
	if (activeBlock & BLOCK_BIG) {
		size = 2;
		scale = SPRITESCALE(0x40);
	}
	else {
		size = 1;
		scale = UNSCALED;
	}

	BlockDefSquare* blockDef = BLOCKDEF(activeBlock & BLOCK_TYPE);
	for (int16_t row = 0, pixelSpread = spread * size; row < 4; row++, squareY += pixelSpread, blockDef += 16u) {
		BlockDefSquare* blockDefSquare = BLOCKDEFROW(blockDef, rotation, row);
		for (int16_t col = 0; col < 4; col++, squareX += pixelSpread, blockDefSquare++) {
			if (*blockDefSquare != DEFBLOCK_EMPTY) {
				DisplayObjectEx(blockObject, squareY, squareX, (uint8_t)palNum, 100u, scale, scale, false);
			}
		}
	}
}
