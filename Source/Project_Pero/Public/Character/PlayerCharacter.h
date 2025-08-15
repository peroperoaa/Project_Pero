// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECT_PERO_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public: 

	APlayerCharacter();

	UPROPERTY(EditAnywhere, Category = "Input|MappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	TObjectPtr<UInputAction> InputMove;
	
	UPROPERTY(EditAnywhere, Category = "Input|Action")
	TObjectPtr<UInputAction> InputJump;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	TObjectPtr<UInputAction> InputRotation;

	UPROPERTY(EditAnywhere, Category = "Agrv")
	float RotationSpeed;

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

private:

	void OnMove(const FInputActionValue& Value);

	void OnJump(const FInputActionValue& Value);

	void OnRotation(const FInputActionValue& Value);

};
