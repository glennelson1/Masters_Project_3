// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_Main.h"

// Sets default values
APCG_Main::APCG_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCG_Main::BeginPlay()
{
	Super::BeginPlay();
	
}
void APCG_Main::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	LevelSeq.Empty();
	Cellref.Empty();
}

void APCG_Main::SpawnGrid()
{
	DeleteGrid();
	RandomChoices.Empty();
	
	for(int i = 0; i < 10; i += 10)
	{
		int32 RandomInt= FMath::RandRange(0, 10);
		RandomChoices.Add(RandomInt);
		if(RandomInt <= 7)
		{
			SpawnFloor(i + 2);
			
		}
		if(RandomInt >= 8) // puts a gap in the floor
			{
			SpawnFloor(i);
			
			}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *LevelSeq);
	//DebugPrintSeed();
}

void APCG_Main::SpawnFloor(int loc)
{
	
	for (int32 X = loc; X <= loc + 10; X++)
	{
		for (int32 Y = 0; Y < 3; Y++)
		{
			FVector SpawnLocation = FVector(X * 100, 0,Y * -100);
			AActor* NewCell;
			NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[0], SpawnLocation, FRotator::ZeroRotator);
			Cellref.Add(NewCell);
			
		}
	}
	
	SpawnBricks(loc);
}

void APCG_Main::SpawnBricks(int loc)
{
	int32 RandomInt= FMath::RandRange(0, 20);
	RandomChoices.Add(RandomInt);
	if(RandomInt <= 10)
	{
		SpawnOb(loc);
	}
	else if(RandomInt >= 11 && RandomInt <= 15)//spawns single block
		{
		FVector SpawnLocation = FVector((loc + 6) * 100, 0,400); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[5], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
		LevelSeq += FString::Printf(TEXT("%d,"), 3);
		
		}
	else if(RandomInt >= 16)// spawns a platform of blocks
		{
		for (int32 X = loc; X < loc + 3; X++)
		{
			FVector SpawnLocation = FVector((X + 5) * 100, 0,400); 
			AActor* NewCell;
			NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
			Cellref.Add(NewCell);
		}
		SpawnPlatforms(loc);
		}
	
}

void APCG_Main::SpawnOb(int loc)
{
	int32 RandomInt= FMath::RandRange(0, 20);
	RandomChoices.Add(RandomInt);
	if(RandomInt >= 11 && RandomInt <= 15)//spawns pipes
		{
		
		FVector SpawnLocation = FVector((loc + 3) * 100, 0, 100);
		FVector SpawnLocation2 = FVector((loc + 9) * 100, 0, 100);
		AActor* NewCell;
		
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[1], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
		
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[2], SpawnLocation2, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
		LevelSeq += FString::Printf(TEXT("%d,"), 1);
		}
	else if(RandomInt >= 16 && RandomInt <= 20) //spawns stairs
		{
		FVector SpawnLocation = FVector((loc + 6) * 100, 0,100); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[3], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
		LevelSeq += FString::Printf(TEXT("%d,"), 2);
		}
	else
	{
		LevelSeq += FString::Printf(TEXT("%d,"), 0);
	}
}

void APCG_Main::SpawnPlatforms(int loc)
{
	int32 RandomInt= FMath::RandRange(0, 100);
	RandomChoices.Add(RandomInt);
	if (RandomInt <= 50)
	{
		if(RandomInt <=30)
		{
			for (int32 X = loc; X < loc + 3; X++)
			{
				FVector SpawnLocation = FVector((X + 10) * 100, 0,800); 
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
				Cellref.Add(NewCell);
			}
			LevelSeq += FString::Printf(TEXT("%d,"), 5);
		}
		if(RandomInt >=31)
		{
			for (int32 X = loc; X < loc + 6; X++)
			{
				FVector SpawnLocation = FVector((X + 10) * 100, 0,800); 
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
				Cellref.Add(NewCell);
				
			}
			LevelSeq += FString::Printf(TEXT("%d,"), 6);
		}
	}
	else
	{
		LevelSeq += FString::Printf(TEXT("%d,"), 4);
	}
}
// Called every frame
void APCG_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

