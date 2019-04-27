#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LudumDare2019/MainGame/Projectiles/Projectile.h"

#include "TDVCharacter.generated.h"


UCLASS(Blueprintable, config = Game)
class ATDVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDVCharacter();

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveForward(float val);
	void MoveRight(float val);

	/**
	 * Called via input (I) to show the invite friend
	 * UI on the client
	**/
	void InviteFriend();


	UFUNCTION(BlueprintNativeEvent)
	void OnFire();

	UFUNCTION(Server, reliable, WithValidation)
	void Server_Fire();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void Server_LoseLife(float amount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void Server_GainLife(float amount);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> _projectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> wTopDownWidget;
	class UUserWidget* TopDownWidget;
};