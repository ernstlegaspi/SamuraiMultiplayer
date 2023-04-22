#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SamuraiAnimInstance.generated.h"

UCLASS()
class SAMURAI_API USamuraiAnimInstance : public UAnimInstance {
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASamuraiCharacter> Samurai;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed;

private:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

};
