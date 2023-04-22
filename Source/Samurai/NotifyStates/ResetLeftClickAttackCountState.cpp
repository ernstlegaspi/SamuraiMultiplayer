#include "ResetLeftClickAttackCountState.h"

#include "Samurai/Character/SamuraiCharacter.h"

void UResetLeftClickAttackCountState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(TObjectPtr<ASamuraiCharacter> Samurai = Cast<ASamuraiCharacter>(MeshComp->GetOwner())) Samurai->SetLeftClickAttackCount(0);
}