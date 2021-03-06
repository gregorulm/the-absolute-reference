#include "Game/Graphics/ShowRowDel.h"
#include "Game/Graphics/Entity.h"
#include "Video/Object.h"
#include "Main/Frame.h"

#define frames values[0]
#define animFrame values[1]

static void UpdateEntityRowDel(Entity* entity);

void ShowRowDel(Player* player, int16_t delRow) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityRowDel;
		entity->data.unionData.player = player;
		entity->frames = 0;
		entity->animFrame = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] - (player->matrixWidth / 2) * 8 + (data->x + 2) * 8;
		data->y = player->screenPos[1] + (player->matrixHeight - delRow - 1) * 8 - (player->matrixHeight - 1) * 8 - 6;
		data->objectTable = OBJECTTABLE_ROWDEL;
		data->palNum = 184u;
	}
}

static void UpdateEntityRowDel(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->animFrame], data->y, data->x, (uint8_t)data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME) {
		if (entity->frames % 2) {
			entity->animFrame++;
		}

		entity->frames++;
		if (entity->animFrame >= 8) {
			FreeEntity(entity);
		}
	}
}
