// Fill out your copyright notice in the Description page of Project Settings.

#include "EnnemiController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/App.h"
#include "Spawner.h"
#include "Amy.h"
#include "AmyZombie.h"

// Sets default values
AEnnemiController::AEnnemiController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnnemiController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), lesPoints);
}

// Called every frame
void AEnnemiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	waveDelay -= DeltaTime;

	if (waveDelay <= 0)
	{
		if (spawnDelay <= 0)
		{
			//Chiffre random
			int rand = FMath::RandRange(0, lesPoints.Num() - 1);

			//Infos du point de spawn
			FVector Location = lesPoints[rand]->GetActorLocation();
			FRotator Rotation = lesPoints[rand]->GetActorRotation();
			FActorSpawnParameters SpawnInfo;

			if (compteurEz != 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "EZ PZ lemon squeeze");

				spawnDelay = 0.3f;
				GetWorld()->SpawnActor<AEnnemiess>(Amy, Location, Rotation, SpawnInfo);
				compteurEz--;
			}
			else if (compteurNormal != 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Normal quoi");

				spawnDelay = 0.3f;
				GetWorld()->SpawnActor<AEnnemiess>(AmyZombie, Location, Rotation, SpawnInfo);
				compteurNormal--;
			}
			else if (compteurHard != 0)
			{
				//spawnDelay = 0.3f;
				//TODO spawner un ennemi hardcore quand on en aura un
			}

			//Si on a plus personne a faire spawn
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Plus d'ennemies, on continue");

				resetVar();
			}
		}
		spawnDelay -= DeltaTime;
	}
}

void AEnnemiController::resetVar()
{
	//D�lai de vague est reset
	waveDelay = 10.f;

	//Les ennemis faciles seront toujours 2 de plus
	nbEzEnnemis += 2;

	//Les ennemis normaux seront un de plus toutes les 4 vagues
	nbNormalEnnemis += 0.25f;

	//Et les ennemis durs seront 1 de plus toutes les dix vagues
	nbHardEnnemis += 0.1f;

	compteurEz = nbEzEnnemis;
	compteurNormal = FMath::Floor(nbNormalEnnemis);
	//compteurHard = FMath::Floor(nbHardEnnemis);

	//On augmente le num�ro de la vague ausssi
	nbWave += 1;
}


//On check si on peux spawner
	//Si le temps entre deux mob est ok
		//On spawn
		//On remet le timer entre mob