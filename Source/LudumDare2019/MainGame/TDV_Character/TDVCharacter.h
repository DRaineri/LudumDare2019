#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LudumDare2019/MainGame/Projectiles/Projectile.h"
#include "MainGame/Monsters/Monster.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

#include "TDVCharacter.generated.h"


UCLASS(Blueprintable, config = Game)
class ATDVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDVCharacter();

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	void MoveForward(float val);
	void MoveRight(float val);

	/**
	 * Called via input (I) to show the invite friend
	 * UI on the client
	**/
	void InviteFriend();

	/**
	 * Methods use to force the TDV to follow
	 * the mouse cursor
	**/
	bool GetMousePositionOnAimingPlane(FVector& IntersectVector) const;
	bool GetPlanePositionAtScreenPosition(
		const FVector2D ScreenPosition,
		FVector& IntersectVector) const;
	void AimUsingMouseCursor();

	UFUNCTION(BlueprintNativeEvent)
	void OnFire();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void Server_LoseLife(float amount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void Server_GainLife(float amount);

	UFUNCTION()
	void OnDetectDefaultAttackCollisionStart(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void OnDetectDefaultAttackCollisionEnd(
		class UPrimitiveComponent* HitComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	UBoxComponent* DefaultAttackCollision;

	UPROPERTY(EditDefaultsOnly, Category = Particles)
	UParticleSystem* DefaultAttackParticles;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> wTopDownWidget;
	class UUserWidget* TopDownWidget;

	// The array of the monsters colliding with the default
	// attack of the player
	TArray<AMonster*> DefaultAttackMonsters;
};