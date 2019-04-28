// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Strike.generated.h"

UCLASS()
class LUDUMDARE2019_API AStrike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = Particles)
	UParticleSystem* ExplosionParticles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION()
		void Explode();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Explode();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Exploded();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ExplodeFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExplodeFX();

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float Damages;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadonly, Category = Config)
		bool bIsExploding;
};
