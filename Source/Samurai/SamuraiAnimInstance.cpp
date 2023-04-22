#include "SamuraiAnimInstance.h"

#include "Samurai/Character/SamuraiCharacter.h"

void USamuraiAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	Samurai = Cast<ASamuraiCharacter>(TryGetPawnOwner());
}

void USamuraiAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	if(!Samurai) return;

	FVector Velocity = Samurai->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}