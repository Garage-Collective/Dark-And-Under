#pragma once

#include "../../Enums.h"

extern const __FlashStringHelper * EnemyNames[];

inline const __FlashStringHelper * getEnemyName(const EnemyType & type) {

	return EnemyNames[static_cast<uint8_t>(type)];
	
}
