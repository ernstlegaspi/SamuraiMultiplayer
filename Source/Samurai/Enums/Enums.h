#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAnimationState : uint8 {
	Default = 0,
	Attack = 1,
	Evade = 2,
	Roll = 3,
	TakingDamage = 4
};