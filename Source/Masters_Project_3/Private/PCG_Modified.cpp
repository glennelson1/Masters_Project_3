// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_Modified.h"

// Sets default values
APCG_Modified::APCG_Modified()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCG_Modified::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCG_Modified::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	LevelSeq.Empty();
}
void APCG_Modified::SpawnGrid()
{
	DeleteGrid();
	m_loc = 0;
	num = 0;
	

	
	
	if (RandomGen)
	{
		for (int i = 0; i <= 15; i++)
		{
			
			int32 RandomInt= FMath::RandRange(0, 6);
			//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), RandomInt);
			switch(RandomInt)
			{
			case 0:
				SpawnEmptySection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 0);
				break;
			case 1:
				SpawnPipeSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 1);
				break;
			case 2:
				SpawnStairsSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 2);
				break;
			case 3:
				SpawnSingleBlockSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 3);
				break;
			case 4:
				SpawnSinglePlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 4);
				break;
			case 5:
				SpawnTwoPlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 5);
				break;
			case 6:
				SpawnTwoLargePlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 6);
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Null"));
				break;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *LevelSeq);
	}
	else
	{
		LevelSeq = "4,1,5,0,6,3,2,0,1,5,6,1,4,4,3,6,";
		for (int i = 0; i < LevelSeq.Len(); i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), LevelSeq[i]);
		
			switch(LevelSeq[i])
			{
			case '0':
				SpawnEmptySection();
				m_loc += 10;
				break;
			case '1':
				SpawnPipeSection();
				m_loc += 10;
				break;
			case '2':
				SpawnStairsSection();
				m_loc += 10;
				break;
			case '3':
				SpawnSingleBlockSection();
				m_loc += 10;
				break;
			case '4':
				SpawnSinglePlatform();
				m_loc += 10;
				break;
			case '5':
				SpawnTwoPlatform();
				m_loc += 10;
			case '6':
				SpawnTwoLargePlatform();
				m_loc += 10;
			
			}
		}
	}
	
	
}
void APCG_Modified::SpawnEmptySection()
{
	int32 RandomInt= FMath::RandRange(0, 10);
	
	if (RandomInt <= 5)
	{
		FVector SpawnLocation = FVector(m_loc  * 100, 0,100); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[6], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
	else
	{
		FVector SpawnLocation = FVector(m_loc * 100, 0,100); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[7], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}

	
}

void APCG_Modified::SpawnPipeSection()
{
	SpawnEmptySection();
	
	FVector SpawnLocation = FVector((m_loc + 3) * 100, 0, 100);
	FVector SpawnLocation2 = FVector((m_loc + 9) * 100, 0, 100);
	AActor* NewCell;
		
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[1], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
		
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[2], SpawnLocation2, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_Modified::SpawnStairsSection()
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + 6) * 100, 0,100); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[3], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_Modified::SpawnSingleBlockSection()
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + 6) * 100, 0,400); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[5], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_Modified::SpawnSinglePlatform()
{
	SpawnEmptySection();
	for (int32 X = m_loc; X < m_loc + 3; X++)
	{
		FVector SpawnLocation = FVector((X + 5) * 100, 0,400); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
	
}

void APCG_Modified::SpawnTwoPlatform()
{
	SpawnEmptySection();
	SpawnSinglePlatform();
	for (int32 X = m_loc; X < m_loc + 3; X++)
	{
		FVector SpawnLocation = FVector((X + 10) * 100, 0,800); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
}

void APCG_Modified::SpawnTwoLargePlatform()
{
	SpawnEmptySection();
	SpawnSinglePlatform();
	
	for (int32 X = m_loc; X < m_loc + 6; X++)
	{
		FVector SpawnLocation = FVector((X + 10) * 100, 0,800); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
				
	}
}



// Called every frame
void APCG_Modified::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

