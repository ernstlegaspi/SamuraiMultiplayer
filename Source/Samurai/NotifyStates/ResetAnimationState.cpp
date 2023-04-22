#include "ResetAnimationState.h"

#include "Samurai/Character/SamuraiCharacter.h"

void UResetAnimationState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(TObjectPtr<ASamuraiCharacter> Samurai = Cast<ASamuraiCharacter>(MeshComp->GetOwner())) Samurai->ResetAnimState();
}
