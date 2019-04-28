// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MainGame/Monsters/Monster.h"
#include "MainGame/FPV_Character/FPVCharacter.h"
#include "MainGame/TDV_Character/TDVCharacter.h"
#include "Components/SphereComponent.h"

#include "LudumDare2019/MainGame/FPV_Character/FPVCharacter.h"
#include "LudumDare2019/MainGame/TDV_Character/TDVCharacter.h"


AProjectile::AProjectile() 
{
	bReplicates = true;
	bReplicateMovement = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Create the Mesh of the projectile
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Hello world
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		if (ProjectileIgnoreTarget == EProjectileIgnoreTarget::EPlayers)
		{
			AMonster* Monster = Cast<AMonster>(OtherActor);
			if (Monster)
				Monster->Server_TakeDamages(Damages);

			AActor* owner = GetOwner();

			ATDVCharacter* tdvCharacter = Cast<ATDVCharacter>(owner);
			if (IsValid(tdvCharacter))
			{
				tdvCharacter->Server_GainLife(tdvCharacter->LifeStealPercentByHit/100 * Damages);
			}

			AFPVCharacter* fpvCharacter = Cast<AFPVCharacter>(owner);
			if (IsValid(fpvCharacter))
			{
				fpvCharacter->Server_GainLife(fpvCharacter->LifeStealPercentByHit/100 * Damages);
			}
		}
		else
		{
			AFPVCharacter* FPVCharacter = Cast<AFPVCharacter>(OtherActor);
			if (FPVCharacter)
			{
				FPVCharacter->Server_LoseLife(Damages);
			}
			else
			{
				ATDVCharacter* TDVCharacter = Cast<ATDVCharacter>(OtherActor);
				if (TDVCharacter)
					TDVCharacter->Server_LoseLife(Damages);
			}
		}

		Destroy();
	}
}