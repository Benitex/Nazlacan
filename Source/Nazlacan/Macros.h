#pragma once

#include "CoreMinimal.h"

#define returnIfNull(InExpression) if (!ensure((IsValid(InExpression)))) return

#define continueIfNull(InExpression) if (!ensure((IsValid(InExpression)))) continue

#define getRandomItemFromArray(Array) Array[FMath::RandRange(0, Array.Num() - 1)]
