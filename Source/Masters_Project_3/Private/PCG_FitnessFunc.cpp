// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_FitnessFunc.h"

// Sets default values
APCG_FitnessFunc::APCG_FitnessFunc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCG_FitnessFunc::BeginPlay()
{
	Super::BeginPlay();
	
}
void APCG_FitnessFunc::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	m_PreviousSect = 0;
	LevelSeq.Empty();
}
void APCG_FitnessFunc::SpawnGrid()
{
	DeleteGrid();
	m_loc = 0;
	
	LevelSeq = "3,3,4,4,0,0,4,5,0,0,6,2,1,3,5,6,4,4,3,2,0,4,4,5,0,5,2,2,1,4,5";

	
	
	if (RandomGen)
	{
		for (int i = 0; i <= 15; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Running"));
			int32 RandomInt= FMath::RandRange(0, 6);
			//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), RandomInt);
			switch(RandomInt)
			{
			case 0:
				SpawnEmptySection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 0);
				m_PreviousSect = 0;
				break;
			case 1:
				SpawnPipeSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 1);
				m_PreviousSect = 1;
				break;
			case 2:
				SpawnStairsSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 2);
				m_PreviousSect = 2;
				break;
			case 3:
				SpawnSingleBlockSection();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 3);
				m_PreviousSect = 3;
				break;
			case 4:
				SpawnSinglePlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 4);
				m_PreviousSect = 4;
				break;
			case 5:
				SpawnTwoPlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 5);
				m_PreviousSect = 5;
				break;
			case 6:
				SpawnTwoLargePlatform();
				m_loc += 10;
				LevelSeq += FString::Printf(TEXT("%d,"), 6);
				m_PreviousSect = 6;
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
void APCG_FitnessFunc::SpawnEmptySection()
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

void APCG_FitnessFunc::SpawnPipeSection()
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

void APCG_FitnessFunc::SpawnStairsSection()
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + 6) * 100, 0,100); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[3], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_FitnessFunc::SpawnSingleBlockSection()
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + 6) * 100, 0,400); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[5], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_FitnessFunc::SpawnSinglePlatform()
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

void APCG_FitnessFunc::SpawnTwoPlatform()
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

void APCG_FitnessFunc::SpawnTwoLargePlatform()
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

void APCG_FitnessFunc::SaveLevelSeqToFile()
{
	// Construct the file path for the LevelSequences file
	FString SaveFilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/AllGoodLevelSeqs.txt");

	
	
	// Ensure the directory exists before trying to save the file
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.DirectoryExists(*FPaths::GetPath(SaveFilePath)))
	{
		FileManager.MakeDirectory(*FPaths::GetPath(SaveFilePath), true);
	}

	// Append the current LevelSeq to the file, with a newline character for separation
	FString ContentToSave = LevelSeq + TEXT("\n"); // Adding a newline character to separate each LevelSeq entry
	bool bWasSuccessful = FFileHelper::SaveStringToFile(ContentToSave, *SaveFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

	// Log whether the operation was successful
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("LevelSeq successfully appended to %s"), *SaveFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to append LevelSeq to %s"), *SaveFilePath);
	}
}


// Called every frame
void APCG_FitnessFunc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

