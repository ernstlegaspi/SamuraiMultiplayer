#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WeaponAttackState.generated.h"

UCLASS()
class SAMURAI_API UWeaponAttackState : public UAnimNotifyState {
	GENERATED_BODY()
	
private:
	TArray<AActor*> ActorsToIgnore;

	TArray<FHitResult> HitResults;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

private:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

};
