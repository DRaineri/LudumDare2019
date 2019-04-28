#include "Spawner.h"

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && _isActiveSpawner)
	{
		GetWorldTimerManager().SetTimer(_timerHandle, this, &ASpawner::Authority_SpawnMonster, SpawnFrequency, true);
	}
}

void ASpawner::Authority_SpawnMonster()
{
	check(HasAuthority());

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Spawn !"));
	FActorSpawnParameters params;
	params.Owner = this;
	MonsterClassToSpawn.LoadSynchronous();
	AMonster* newMonster = GetWorld()->SpawnActor<AMonster>(MonsterClassToSpawn.Get(), GetActorLocation(), GetActorRotation(), params);
	if (IsValid(newMonster))
	{
		newMonster->LaunchCharacter(GetActorForwardVector() * 100, true, true);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Spawn down !"));

		_nbMonsterSpawnedSinceLastActivation++;

		if (_nbMonsterSpawnedSinceLastActivation >= NumberOfMonsterToSpawn)
		{
			Authority_SetActiveSpawner(false);
		}
	}
}

void ASpawner::Authority_SetActiveSpawner(bool isActive)
{ 
	_isActiveSpawner = isActive; 

	if (isActive)
	{
		if (!_timerHandle.IsValid())
		{
			_nbMonsterSpawnedSinceLastActivation = 0;
			GetWorldTimerManager().SetTimer(_timerHandle, this, &ASpawner::Authority_SpawnMonster, SpawnFrequency, true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(_timerHandle);
	}
}
