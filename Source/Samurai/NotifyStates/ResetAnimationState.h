#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ResetAnimationState.generated.h"

UCLASS()
class SAMURAI_API UResetAnimationState : public UAnimNotifyState {
	GENERATED_BODY()
	
private:
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
