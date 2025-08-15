// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	RotationSpeed = 1.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(InputMappingContext);
	TObjectPtr<APlayerController> PC = CastChecked<APlayerController>(GetController());
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(InputMappingContext, 0);
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputMove)
		EnhancedInputComponent->BindAction(InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);
	if (InputJump)
		EnhancedInputComponent->BindAction(InputJump, ETriggerEvent::Triggered, this, &APlayerCharacter::OnJump);
	if (InputRotation)
		EnhancedInputComponent->BindAction(InputRotation, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRotation);
}

void APlayerCharacter::OnMove(const FInputActionValue& Value)
{

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 找出是哪个方向向前 获取控制旋转
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		//ue坐标系 前面 是x 垂直是z水平 是y
		// 获取向前向量
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    
		//获取向右向量
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void APlayerCharacter::OnJump(const FInputActionValue& Value)
{
	Jump();
}

void APlayerCharacter::OnRotation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);//围绕x轴 [ue-x代表向前的水平线]偏航叫 控制左右旋转 z代表高度垂直线
		AddControllerPitchInput(-LookAxisVector.Y * RotationSpeed);//围绕y轴 [ue是水平线]俯仰角 roll  在unreal中，pitch 对应于y轴，yaw对应于z轴，roll对应于x轴。
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f"), LookAxisVector.X, LookAxisVector.Y));
}
