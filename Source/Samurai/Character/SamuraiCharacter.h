#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"

#include "Samurai/Enums/Enums.h"

#include "SamuraiCharacter.generated.h"

class UAnimMontage;
class UInputAction;
class UProgressBarManager;

UCLASS()
class SAMURAI_API ASamuraiCharacter : public ABaseCharacter {
	GENERATED_BODY()

/*----- START OF PRIVATE VARIABLES ------*/
private:
	/*----- START OF UPROPERTY(EditAnywhere) ------*/
	/*----- START OF ANIM MONTAGES ------*/
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_EvadeBack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_EvadeFront;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_EvadeLeft;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_EvadeRight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_LeftClickAttack1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_LeftClickAttack2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_LeftClickAttack3;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_RightClickAttack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AM_RollFront;
	/*----- END OF ANIM MONTAGES ------*/

	/*----- START OF INPUTS ------*/
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MouseLeftClickAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MouseRightClickAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> SamuraiMappingContext;
	/*----- END OF INPUTS ------*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<UProgressBarManager> HealthBarWidget;

	UPROPERTY(EditAnywhere)
	float RunSpeed;

	UPROPERTY(EditAnywhere)
	float SprintSpeed;

	UPROPERTY(EditAnywhere)
	float MoveValueMultiplier;
	/*----- END OF UPROPERTY(EditAnywhere) ------*/

	/*----- START OF UPROPERTY(VisibleAnywhere) ------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraFollow;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;
	/*----- END OF UPROPERTY(VisibleAnywhere) ------*/

	/*----- Start OF UPROPERTY(Replicated) ------*/
	UPROPERTY(Replicated)
	bool bIsSprintPressed;

	UPROPERTY(Replicated)
	float MoveValue;

	/*----- END OF UPROPERTY(Replicated) ------*/

	/*----- START OF VARIABLES THAT HAVE NO MACRO ------*/
	TObjectPtr<class UAnimInstance> SamuraiAnimInstance;

	TObjectPtr<UProgressBarManager> HealthBar;

	FTimerHandle TimerHandle;
	FVector2D PrevEvadeAxis;

	EAnimationState AnimState;

	uint8 LeftClickAttackCount;

	float Health;

	bool bIsEvadePressed;
	/*----- END OF VARIABLES THAT HAVE NO MACRO ------*/
/*----- END OF PRIVATE VARIABLES ------*/

/*----- START OF PUBLIC FUNCTIONS ------*/
public:
	ASamuraiCharacter();

	void ResetAnimState();
	/*----- START OF GETTER AND SETTERS ------*/
	TObjectPtr<UProgressBarManager> GetHealthBar();

	void SetAnimState(EAnimationState State);

	void SetLeftClickAttackCount(uint8 Count);
	void SetTick(bool ToggleTick);
	/*----- END OF GETTER AND SETTERS ------*/

/*----- END OF PUBLIC FUNCTIONS ------*/

/*----- START OF PRIVATE FUNCTIONS ------*/
private:
	bool IsDefaultState();

	void PlayAnim(TObjectPtr<UAnimMontage> AM);
	void ResetEvade();

	void SetCharacterSpeed(float Speed);

	/*----- START OF MUTLIPLAYER FUNCTIONS ------*/
	UFUNCTION(Server, Reliable)
	void EvadeServer(FVector2D EvadeAxis);

	UFUNCTION(NetMulticast, Reliable)
	void EvadeMulticast(FVector2D EvadeAxis);

	UFUNCTION(Server, Reliable)
	void LeftClickAttackServer();

	UFUNCTION(NetMulticast, Reliable)
	void LeftClickAttackMulticast();

	UFUNCTION(Server, Reliable)
	void MoveServer();

	UFUNCTION(Server, Reliable)
	void RightClickAttackServer();

	UFUNCTION(NetMulticast, Reliable)
	void RightClickAttackMulticast();

	UFUNCTION(Server, Reliable)
	void RollServer();

	UFUNCTION(NetMulticast, Reliable)
	void RollMulticast();

	UFUNCTION(Server, Reliable)
	void SprintServer(bool IsSprintPressed);
	/*----- END OF MUTLIPLAYER FUNCTIONS ------*/


	/*----- START OF INPUT BIND FUNCTIONS ------*/
	void MouseLeftClick();
	void MouseRightClick();
	void MoveReleased();
	void Roll();
	void SprintStart();
	void SprintReleased();

	void EvadeStart(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	/*----- END OF INPUT BIND FUNCTIONS ------*/

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
/*----- END OF PRIVATE FUNCTIONS ------*/

};
