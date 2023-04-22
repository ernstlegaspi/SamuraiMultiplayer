#include "SamuraiCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

#include "Samurai/Widgets/ProgressBarManager.h"

/*




	Good day. I just want to ask a few questions. If you are reading this
	and knows the solution for my question, kindly email me at

	legaspiernstdanielle@gmail.com
	Thank you so much.

	1. How can I implement a camera shake using c++?

	2. In the SamuraiCharacter class, line 135, the code is HealthBar->SetBar(Health);
	I would suggest uncommenting this line and play it in the editor. Make sure that
	you are playing as a listen server and have 2 clients. Now,
	Left or right click, and hit an enemy, the result for this should crash
	the game. Do you know what is the error? thank you so much.

	I am practicing unreal engine multiplayer, it will be a huge help if you know this fix. Thank you so much.



*/


ASamuraiCharacter::ASamuraiCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());

	CameraFollow = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Follow"));
	CameraFollow->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ASamuraiCharacter::BeginPlay() {
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(SamuraiMappingContext, 0);
		}
	}

	// check if HealthBarWidget is not null
	if(HealthBarWidget && IsLocallyControlled()) {
		HealthBar = CreateWidget<UProgressBarManager>(GetWorld(), HealthBarWidget);
		HealthBar->AddToViewport();
	}

	// get the animinstance
	SamuraiAnimInstance = GetMesh()->GetAnimInstance();

	// disable the tick function once the game begins
	SetTick(false);

	// set the character speed to 0 so that when the player start pressing the move keys, it will smoothly go from 0 to RunSpeed
	SetCharacterSpeed(0.f);
	
	Health = GetCurrentHealth();
}

void ASamuraiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASamuraiCharacter, bIsSprintPressed);
	DOREPLIFETIME(ASamuraiCharacter, MoveValue);
}

void ASamuraiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		// Mouse Left and Right Click
		EnhancedInputComponent->BindAction(MouseLeftClickAction, ETriggerEvent::Started, this, &ThisClass::MouseLeftClick);
		EnhancedInputComponent->BindAction(MouseRightClickAction, ETriggerEvent::Started, this, &ThisClass::MouseRightClick);

		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveReleased);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::EvadeStart);

		// Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ThisClass::Roll);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::SprintReleased);
	}
}

void ASamuraiCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// check if the sprint key is released
	if(!bIsSprintPressed) {
		/*
			check if the MoveValue is greater than RunSpeed.
			it will return true since the value of MoveValue is greater than RunSpeed when shift is pressed
		*/
		if(MoveValue > RunSpeed) {
			// subtract it to the deltatime * MoveValueMultiplier to smoothly transition from sprinting animation to running animation
			MoveValue -= DeltaTime * MoveValueMultiplier;
			SetCharacterSpeed(MoveValue);
		}
		else {
			// SetTick(false);
		}
	}

	if(GetIsAttacked()) {
		if(Health > (GetCurrentHealth() - .1f)) {
			Health -= DeltaTime;
			// HealthBar->SetBar(Health);
		}
		else {
			SetCurrentHealth(Health);
			SetIsAttacked(false);
			SetTick(false);
			AnimState = EAnimationState::Default;
		}
	}
}

void ASamuraiCharacter::Look(const FInputActionValue& Value) {
	if(!Controller) return;

	const FVector2D Look = Value.Get<FVector2D>();

	AddControllerYawInput(Look.X);
	AddControllerPitchInput(-Look.Y);
}

void ASamuraiCharacter::ResetEvade() {
	bIsEvadePressed = false;
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

/*----- START OF ATTACK FUNCTIONS ------*/
void ASamuraiCharacter::MouseLeftClick() {
	// check if the Controller and IsDefaultState is not a nullptr, if it is just return;
	if(!Controller || !IsDefaultState()) return;

	LeftClickAttackServer();

	// set the current anim state
	AnimState = EAnimationState::Attack;
}

void ASamuraiCharacter::MouseRightClick() {
	// check if the Controller and IsDefaultState is not a nullptr, if it is just return;
	if(!Controller || !IsDefaultState()) return;

	RightClickAttackServer();

	// set the current anim state
	AnimState = EAnimationState::Attack;
}
/*----- END OF ATTACK FUNCTIONS ------*/

/*----- START OF MOVEMENT FUNCTIONS ------*/
void ASamuraiCharacter::EvadeStart(const FInputActionValue& Value) {
	/*
		double tap a move key to evade
		pressing the second evade key should be fast
		if W is pressed, the second evade key should also be W
		W W. The interval is set to 0.2f, pressing the W W should be fast
	*/

	// check if the Controller and IsDefaultState is not a nullptr, if it is just return;
	if(!Controller || !IsDefaultState()) return;

	const FVector2D EvadeAxis = Value.Get<FVector2D>();

	// check if the PrevEvadeAxis value is not equal to the current EvadeAxis value pressed
	// reset it if it not equal
	if(PrevEvadeAxis != EvadeAxis) ResetEvade();

	/*
		check if any of the evade key is pressed
		first evade key pressed will go to the else statement
		second evade key pressed will go to the if statement
	*/
	if(bIsEvadePressed) {
		EvadeServer(EvadeAxis);

		// reset values once the evade animation is played
		AnimState = EAnimationState::Evade;
		ResetEvade();
	}
	else {
		bIsEvadePressed = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// set a timer when first evade key is pressed
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::ResetEvade, .15f, false);
	}

	// set the PrevEvadeAxis to the current evade axis
	PrevEvadeAxis = EvadeAxis;
}

void ASamuraiCharacter::Move(const FInputActionValue& Value) {
	if(!Controller) return;

	const FVector2D Move = Value.Get<FVector2D>();

	// check whether the player pressed shift for sprinting
	bool bIsSprinting = bIsSprintPressed ? MoveValue < SprintSpeed : MoveValue < RunSpeed;

	// check if the MoveValue is less than the runspeed or sprintspeed
	if(bIsSprinting) {
		MoveValue += GetWorld()->GetDeltaSeconds() * MoveValueMultiplier;
		SetCharacterSpeed(MoveValue);
		MoveServer();
	}

	const FRotator NewYawRotation(0, GetControlRotation().Yaw, 0);

	const FVector Forward = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Move.Y);
	AddMovementInput(Right, Move.X);
}

void ASamuraiCharacter::MoveReleased() {
	MoveValue = 0.f;
}

// Press left ctrl to roll
void ASamuraiCharacter::Roll() {
	if(!Controller || !IsDefaultState()) return;

	RollServer();

	AnimState = EAnimationState::Roll;
}

// Press left shift to start sprinting
void ASamuraiCharacter::SprintStart() {
	bIsSprintPressed = true;
	SetTick(false);
	SprintServer(true);
}

void ASamuraiCharacter::SprintReleased() {
	bIsSprintPressed = false;
	SetTick(true);
	SprintServer(false);
}
/*----- END OF MOVEMENT FUNCTIONS ------*/

/*----- START OF MUTLIPLAYER FUNCTIONS ------*/
void ASamuraiCharacter::EvadeServer_Implementation(FVector2D EvadeAxis) {
	EvadeMulticast(EvadeAxis);
}

void ASamuraiCharacter::EvadeMulticast_Implementation(FVector2D EvadeAxis) {
	// check if A or D key is pressed only
	if(EvadeAxis.Y == 0) {
		// play evade anim montage depending on the axis pressed
		PlayAnim(EvadeAxis.X == -1 ? AM_EvadeLeft : EvadeAxis.X == 1 ? AM_EvadeRight : nullptr);
	}
	// check if W or S key is pressed only
	else {
		// play evade anim montage depending on the axis pressed
		PlayAnim(EvadeAxis.Y == -1 ? AM_EvadeBack : EvadeAxis.Y == 1 ? AM_EvadeFront : nullptr);
	}
}

void ASamuraiCharacter::LeftClickAttackServer_Implementation() {
	LeftClickAttackMulticast();
}

void ASamuraiCharacter::LeftClickAttackMulticast_Implementation() {
	/*
		increment the LeftClickAttackCount variable per left mouse click
		check whether it is greater than 3. 3 is the number of Attack AnimMontages
		if it is greater than 3, reset the value and play the first LeftClickAttack anim montage
	*/
	if(++LeftClickAttackCount > 3) LeftClickAttackCount = 1;

	// player anim montages
	PlayAnim(LeftClickAttackCount == 1 ? AM_LeftClickAttack1 : LeftClickAttackCount == 2 ? AM_LeftClickAttack2 : AM_LeftClickAttack3);
}

void ASamuraiCharacter::MoveServer_Implementation() {
	// increment the MoveValue to get that smooth transition for character speed
	MoveValue += GetWorld()->GetDeltaSeconds() * MoveValueMultiplier;
	SetCharacterSpeed(MoveValue);
}

void ASamuraiCharacter::RightClickAttackServer_Implementation() {
	RightClickAttackMulticast();
}

void ASamuraiCharacter::RightClickAttackMulticast_Implementation() {
	// player anim montage
	PlayAnim(AM_RightClickAttack);
}

void ASamuraiCharacter::RollServer_Implementation() {
	RollMulticast();
}

void ASamuraiCharacter::RollMulticast_Implementation() {
	PlayAnim(AM_RollFront);
}

void ASamuraiCharacter::SprintServer_Implementation(bool IsSprintPressed) {
	bIsSprintPressed = IsSprintPressed;
	SetTick(!IsSprintPressed);
}
/*----- END OF MUTLIPLAYER FUNCTIONS ------*/

/*----- START OF GETTER AND SETTERS ------*/
void ASamuraiCharacter::SetAnimState(EAnimationState State) {
	AnimState = State;
}

void ASamuraiCharacter::SetLeftClickAttackCount(uint8 Count) {
	LeftClickAttackCount = Count;
}
/*----- END OF GETTER AND SETTERS ------*/

bool ASamuraiCharacter::IsDefaultState() {
	return AnimState == EAnimationState::Default;
}

void ASamuraiCharacter::PlayAnim(TObjectPtr<UAnimMontage> AM) {
	SamuraiAnimInstance->Montage_Play(AM);
}

void ASamuraiCharacter::ResetAnimState() {
	AnimState = EAnimationState::Default;
}

void ASamuraiCharacter::SetCharacterSpeed(float Speed) {
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ASamuraiCharacter::SetTick(bool ToggleTick) {
	SetActorTickEnabled(ToggleTick);
}