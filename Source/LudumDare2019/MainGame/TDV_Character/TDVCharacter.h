#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDVCharacter.generated.h"


UCLASS(Blueprintable, config = Game)
class ATDVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDVCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveForward(float val);
	void MoveRight(float val);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};