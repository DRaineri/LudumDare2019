// Fill out your copyright notice in the Description page of Project Settings.

#include "Strike.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MainGame/FPV_Character/FPVCharacter.h"
#include "MainGame/TDV_Character/TDVCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AStrike::AStrike()
{
	// Create the collision component
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(75.f);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStrike::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&AStrike::Explode,
		1.0f,
		false);
}

void AStrike::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStrike, bIsExploding);
}

// Called every frame
void AStrike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStrike::Explode()
{
	if (HasAuthority())
	{
		Multicast_ExplodeFX();
		Server_Explode();
	}
	else
	{
		Server_ExplodeFX();
	}
}

void AStrike::Server_Explode_Implementation()
{
	// TODO : Deals damages to the players in range
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		AFPVCharacter* FPVCharacter = Cast<AFPVCharacter>(Actor);
		if (FPVCharacter)
			FPVCharacter->Server_LoseLife(Damages);

		ATDVCharacter* TDVCharacter = Cast<ATDVCharacter>(Actor);
		if (TDVCharacter)
			TDVCharacter->Server_LoseLife(Damages);
 	}

	bIsExploding = true;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		this,
		&AStrike::Server_Exploded,
		0.2f,
		false);
}

void AStrike::Server_Exploded_Implementation()
{
	Destroy();
}

bool AStrike::Server_Exploded_Validate()
{
	return true;
}

bool AStrike::Server_Explode_Validate()
{
	return true;
}

void AStrike::Server_ExplodeFX_Implementation()
{
	Multicast_ExplodeFX();
}

bool AStrike::Server_ExplodeFX_Validate()
{
	return true;
}

void AStrike::Multicast_ExplodeFX_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ExplosionParticles,
		GetActorLocation(),
		GetActorRotation(),
		true
	);
}