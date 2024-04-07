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
		LevelSeq = "1,6,2,1,3,6,3,0,6,6,4,5,5,2,1,1,";
		for (int i = 0; i <= 30; i++)
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
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *LevelSeq);
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
	FString SaveFilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/AllGoodLevelSeqs.txt");
	int32 ChunkSize = 30; // Size of each small sequence //11

	// Ensure the directory exists before trying to save the file
	FString SaveDirectory = FPaths::GetPath(SaveFilePath);
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.DirectoryExists(*SaveDirectory))
	{
		FileManager.MakeDirectory(*SaveDirectory, true);
	}

	// Break down LevelSeq into smaller sequences of ChunkSize and save each
	for (int32 StartIndex = 0; StartIndex < LevelSeq.Len(); StartIndex += ChunkSize)
	{
		// Extract a substring of ChunkSize length from LevelSeq
		FString SubSequence = LevelSeq.Mid(StartIndex, ChunkSize);
        
		// Ensure it ends with a newline for readability in the file
		FString ContentToSave = SubSequence + TEXT("\n");

		// Attempt to append the content to the specified file
		bool bWasSuccessful = FFileHelper::SaveStringToFile(ContentToSave, *SaveFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

		// Optionally, log each chunk save attempt
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Subsequence successfully appended to %s"), *SaveFilePath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to append Subsequence to %s"), *SaveFilePath);
		}
	}
}


// Called every frame
void APCG_FitnessFunc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

