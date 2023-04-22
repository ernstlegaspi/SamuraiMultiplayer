#include "WeaponAttackState.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Samurai/Character/SamuraiCharacter.h"

void UWeaponAttackState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(TObjectPtr<ASamuraiCharacter> Samurai = Cast<ASamuraiCharacter>(MeshComp->GetOwner())) {
		if(Samurai->IsLocallyControlled()) {
			FVector Start = MeshComp->GetSocketLocation("katana_start");
			FVector End = MeshComp->GetSocketLocation("katana_end");

			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

			ActorsToIgnore.Add(MeshComp->GetOwner());

			for(auto HitActor : HitResults) {
				if(TObjectPtr<ASamuraiCharacter> HitSamurai = Cast<ASamuraiCharacter>(HitActor.GetActor())) {
					if(!HitSamurai->GetIsAttacked()) {
						HitSamurai->SetAnimState(EAnimationState::TakingDamage);
						HitSamurai->SetIsAttacked(true);
						HitSamurai->SetTick(true);
					}
				}
			}

			UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp->GetWorld(),
				Start,
				End,
				50.f,
				ObjectTypes,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResults,
				true);
			// UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp->GetWorld(), Start, End, 50.f, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true, FLinearColor::Red, FLinearColor::Green, 1.f);
		}
	}
}