#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equipment.generated.h"

UINTERFACE(MinimalAPI)
class UEquipment : public UInterface {
	GENERATED_BODY()
};

class NAZLACAN_API IEquipment {
	GENERATED_BODY()

public:
	virtual bool DestroyEquipment(bool bNetForce = false, bool bShouldModifyLevel = true) = 0;
	virtual bool AttachEquipment(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, FName SocketName = NAME_None) = 0;
};
