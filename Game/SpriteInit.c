#include "SpriteInit.h"
#include "Video.h"
#include "LibC.h"

// Cases 0 and 1 appear to initialize the first two sprites with some settings.
// Case 2 adds an unscaled sprite (similar to DisplayObject).
// Case 3 does nothing.
// Case 4 appears to be like DisplayObjectEx, as it supports scaling inputs.
// Case 5 appears similar to case 4, with some differences in its inputs.
// TODO: Looks like a good name could be "SetSprite(SpriteSetType type, SpriteSetData* data)".
// This appears to be a Psikyo-written function for writing sprites.
void SetSprite(uint16_t type, struct_0x606006C* arg1) {
	switch (type) {
	case 0u:
		NumSprites = SPRITE_FIRST;
		_0x6061932.tempSprite[0] = 0xFFF0u;
		_0x6061932.tempSprite[1] = 0xFFF0u;
		_0x6061932.tempSprite[2] = 0x003Fu;
		_0x6061932.tempSprite[3] = 0x003Fu;
		_0x6061932.tempSprite[4] = 0x0000u;
		_0x6061932.tempSprite[5] = 0x0000u;
		Sprites[0][0] = 0xFFF0u;
		Sprites[0][1] = 0xFFF0u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0000u;
		Sprites[0][6] = _0x6061932.tempSprite[6];
		Sprites[0][7] = _0x6061932.tempSprite[7];
		Sprites[1][0] = 0xFFF0u;
		Sprites[1][1] = 0xFFF0u;
		Sprites[1][2] = 0x003Fu;
		Sprites[1][3] = 0x003Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0000u;
		Sprites[1][6] = _0x6061932.tempSprite[6];
		Sprites[1][7] = _0x6061932.tempSprite[7];
		return;

	case 1u:
		NumSprites = SPRITE_FIRST;
		_0x6061932.tempSprite[0] = 0x0000u;
		_0x6061932.tempSprite[1] = 0x0200u;
		_0x6061932.tempSprite[2] = 0x0D3Fu;
		_0x6061932.tempSprite[3] = 0x313Fu;
		_0x6061932.tempSprite[4] = 0x0000u;
		_0x6061932.tempSprite[5] = 0x0003u;
		Sprites[0][0] = 0x01C0u;
		Sprites[0][1] = 0x0200u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0003u;
		Sprites[0][6] = _0x6061932.tempSprite[6];
		Sprites[0][7] = _0x6061932.tempSprite[7];
		Sprites[1][0] = 0x0000u;
		Sprites[1][1] = 0x0200u;
		Sprites[1][2] = 0x0D3Fu;
		Sprites[1][3] = 0x313Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0003u;
		Sprites[1][6] = _0x6061932.tempSprite[6];
		Sprites[1][7] = _0x6061932.tempSprite[7];
		return;
	case 2u:
		_0x6061932.tempSprite[0] = arg1->y;
		_0x6061932.tempSprite[1] = arg1->x;
		// Set sprite's vertical flip, sprite priority, height, and height scale.
		_0x6061932.tempSprite[2] = (arg1->objectTable[0][2] & (OBJECT_SPRPRI | OBJECT_H)) | UNSCALED;
		// Set sprite's horizontal flip, background priority, width, and width scale.
		_0x6061932.tempSprite[3] = (arg1->flipXBgPri << 12) | (arg1->objectTable[0][3] & ~OBJECT_SCALEX) | UNSCALED;
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		_0x6061932.tempSprite[4] = OBJECT_TOPALNUM(arg1->palNum) | (arg1->objectTable[0][4] & ~OBJECT_PALNUM);
		// Set sprite's lower 16 tile number bits.
		_0x6061932.tempSprite[5] = arg1->objectTable[0][5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 3u:
		return;

	case 4u:
		_0x6061932.tempSprite[0] = arg1->word0;
		_0x6061932.tempSprite[1] = arg1->word1;
		// Set sprite's vertical flip, unknown word 2 bit, sprite priority, height, and height scale.
		_0x6061932.tempSprite[2] =
			(arg1->objectTable[0][2] & (OBJECT_UNKNOWN0 | OBJECT_H)) |
			(arg1->flipYSprPriH << 8) |
			arg1->scaleY;
		// Set sprite's horizontal flip, unknown word 3 bit, background
		// priority, width, and width scale.
		// Also or's in a bitmask for some reason, that can set bits [15, 8] of
		// the word. It appears the or'd in mask is intended to set flipX, as
		// everything else in word 3 is set here.
		_0x6061932.tempSprite[3] =
			(arg1->flipXBgPriW << 9) |
			(arg1->objectTable[0][3] & (OBJECT_UNKNOWN1 | OBJECT_W)) |
			OBJECT_TOBGPRI(arg1->flipXBgPri) |
			OBJECT_TOSCALEX(arg1->scaleX);
		uint16_t palNum;
		if (arg1->palNum == 0u) {
			palNum = OBJECT_GETPALNUM(arg1->objectTable);
		}
		else {
			palNum = arg1->palNum;
		}
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		_0x6061932.tempSprite[4] = OBJECT_TOPALNUM(palNum) | (arg1->objectTable[0][4] & ~OBJECT_PALNUM);
		// Set sprite's lower 16 tile number bits.
		_0x6061932.tempSprite[5] = arg1->objectTable[0][5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 5u:
		// Set X and Y position of the sprite, but clear all other bits in words 0 and 1.
        _0x6061932.tempSprite[0] = arg1->word0 & OBJECT_Y;
        _0x6061932.tempSprite[1] = arg1->word1 & OBJECT_X;
		// Set sprite's vertical flip, unknown word 2 bit, sprite priority, height, and height scale.
        _0x6061932.tempSprite[2] = (arg1->flipYSprPriH << 8) | arg1->scaleY;
		// Set sprite's horizontal flip, unknown word 3 bit, background priority, width, and width scale.
        _0x6061932.tempSprite[3] = (arg1->flipXBgPriW << 8) | arg1->scaleX;
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		_0x6061932.tempSprite[4] = OBJECT_TOPALNUM(arg1->palNum) | arg1->bppAlphaTileTop;
		// Set sprite's lower 16 tile number bits.
        _0x6061932.tempSprite[5] = arg1->objectTable[0][5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 6u: {
		ObjectData* animFrameObject = &arg1->objectTable[arg1->animFrame];
		uint8_t flipXBgPriW;
		if (arg1->_0x20 < 4) {
			_0x6061932.tempSprite[0] = OBJECT_TOY(arg1->y + (*animFrameObject)[0]);
			_0x6061932.tempSprite[1] = OBJECT_TOX(arg1->x + (*animFrameObject)[1]);
			// Disable vertical flip, clear unknown word 2 bit, set sprite
			// prioity to zero, and set sprite height to one.
			_0x6061932.tempSprite[2] &= OBJECT_SCALEY;
			// Set background priority to one.
			flipXBgPriW = 0x10u;
		}
		else {
			if (arg1->verticalHorizontal & 0x8000u) {
				_0x6061932.tempSprite[0] = OBJECT_TOY(arg1->y - (int8_t)(*animFrameObject)[0] - 16);
			}
			else {
				_0x6061932.tempSprite[0] = OBJECT_TOY(arg1->y + (*animFrameObject)[0]);
			}

			if (arg1->verticalHorizontal & 0x0080u) {
				_0x6061932.tempSprite[1] = OBJECT_TOX(arg1->x - (int8_t)(*animFrameObject)[1] - 16);
			}
			else {
				_0x6061932.tempSprite[1] = OBJECT_TOX(arg1->x + (*animFrameObject)[1]);
			}
			_0x6061932.tempSprite[2] |= arg1->verticalHorizontal & 0xFF00u;
			flipXBgPriW = (uint8_t)arg1->verticalHorizontal | 0x10u;
		}

		_0x6061932.tempSprite[3] |= (uint16_t)flipXBgPriW << 8;
		_0x6061932.tempSprite[4] |= (*animFrameObject)[4] & OBJECT_PALNUM;
		_0x6061932.tempSprite[5] = (*animFrameObject)[5];
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;
	}

	case 7u:
		_0x6061932.tempSprite[0] = OBJECT_TOY(arg1->y + arg1->objectTable[0][0]);
		_0x6061932.tempSprite[1] = OBJECT_TOX(arg1->x + arg1->objectTable[0][1]);
		_0x6061932.tempSprite[2] |= (*arg1->objectTable)[2] & ~OBJECT_SCALEY;
		_0x6061932.tempSprite[3] |= (arg1->objectTable[0][3] & ~OBJECT_SCALEX) | OBJECT_TOBGPRI(1);
		_0x6061932.tempSprite[4] = arg1->objectTable[0][4];
		_0x6061932.tempSprite[5] = arg1->objectTable[arg1->animFrame][5];
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 8u:
		_0x6061932.tempSprite[0] = OBJECT_TOY(arg1->y + arg1->objectTable[0][0]);
		_0x6061932.tempSprite[1] = OBJECT_TOX(arg1->x + arg1->objectTable[0][1]);
		_0x6061932.tempSprite[2] = (arg1->objectTable[0][2] & ~OBJECT_SCALEY) | UNSCALED;
		_0x6061932.tempSprite[3] = (arg1->objectTable[0][3] & ~OBJECT_SCALEX) | UNSCALED | OBJECT_TOBGPRI(2);
		_0x6061932.tempSprite[4] = arg1->objectTable[0][4];
        _0x6061932.tempSprite[5] = arg1->objectTable[0][5];
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 9u:
		_0x6061932.tempSprite[0] = OBJECT_TOY(200);
		_0x6061932.tempSprite[1] = OBJECT_TOX(10);
		_0x6061932.tempSprite[2] = UNSCALED;
		_0x6061932.tempSprite[3] = OBJECT_TOBGPRI(3u) | UNSCALED;
		_0x6061932.tempSprite[4] = OBJECT_TOTILETOP(0x1Fu);
		_0x6061932.tempSprite[5] = OBJECT_TOTILEBOTTOM(0x1Fu);
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 10u:
		_0x6061932.tempSprite[0] = arg1->word0;
		_0x6061932.tempSprite[1] = arg1->word1;
		_0x6061932.tempSprite[2] = (arg1->flipYSprPriH << 8) | UNSCALED;
		_0x6061932.tempSprite[3] = (arg1->flipXBgPriW << 8) | UNSCALED;
		_0x6061932.tempSprite[4] = OBJECT_TOPALNUM(arg1->palNum) | OBJECT_TOTILETOP(0xFu);
		_0x6061932.tempSprite[5] = OBJECT_TOTILEBOTTOM(0xFu);
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	case 11u:
		_0x6061932.tempSprite[0] = arg1->word0;
		_0x6061932.tempSprite[1] = arg1->word1;
		_0x6061932.tempSprite[2] = (arg1->flipYSprPriH << 8) | OBJECT_TOSCALEY(arg1->scaleY);
		_0x6061932.tempSprite[3] = (arg1->flipXBgPriW << 8) | OBJECT_TOSCALEX(arg1->scaleX);
		_0x6061932.tempSprite[4] = OBJECT_TOPALNUM(arg1->palNum) | arg1->bppAlphaTileTop;
		_0x6061932.tempSprite[5] = arg1->tileBottom;
		SPRITE_COPY(Sprites[NumSprites], _0x6061932.tempSprite);
		NumSprites++;
		return;

	default:
		// BUG: Original didn't have a default case, and would have undefined
		// behavior. Fixed that here.
		return;
	}
}