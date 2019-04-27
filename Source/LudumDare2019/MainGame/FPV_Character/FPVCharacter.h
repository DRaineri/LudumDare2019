// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LudumDare2019/MainGame/Projectiles/Projectile.h"

#include "FPVCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AFPVCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPSCameraComponent;

public:
	AFPVCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Called via input (I) to show the invite friend
	 * UI on the client
	 */
	void InviteFriend();

	UFUNCTION(BlueprintNativeEvent)
	void OnFire();

	UFUNCTION(Server, reliable, WithValidation)
	void Server_Fire();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> _projectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> wFirstPersonWidget;
	class UUserWidget* FirstPersonWidget;

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFPSCameraComponent() const { return FPSCameraComponent; }

};

