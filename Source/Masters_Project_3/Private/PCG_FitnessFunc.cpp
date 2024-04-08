// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_FitnessFunc.h"

// Sets default values
APCG_FitnessFunc::APCG_FitnessFunc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SectionProbabilities.Add(0, 10.0f); // Empty
	SectionProbabilities.Add(1, 10.0f); // Pipes
	SectionProbabilities.Add(2, 10.0f); // Stairs
	SectionProbabilities.Add(3, 10.0f); // Single Block
	SectionProbabilities.Add(4, 10.0f); // One Platform
	SectionProbabilities.Add(5, 10.0f); // Small Platforms
	SectionProbabilities.Add(6, 10.0f); // Large Platforms
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
	m_emptySect= 0, m_pipeSect =0, m_StairsSect =0, m_SingleBlockSect =0,m_singlePlat = 0, m_SmallPlatSect = 0;
	m_LargePlatSect = 0;
	m_PreviousSect = 0;
	m_Fitness = 0;
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
			  // Call DetermineProbability to adjust the spawn probabilities based on the previous section
            DetermineProbability();

            // Select a section to spawn based on the adjusted probabilities
            int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);

            // Spawn the selected section
            switch(SelectedSection)
            {
            case 0:
                SpawnEmptySection();
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 0);
                m_emptySect++;
                m_PreviousSect = 0;
                break;
            case 1: //pipes
                SpawnPipeSection();
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 1);
                m_pipeSect++;
                m_PreviousSect = 1;
                break;
            case 2: //stairs 
                SpawnBlockSection(3, 6, 100);
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 2);
                m_StairsSect++;
                m_PreviousSect = 2;
                break;
            case 3: //single block
                SpawnBlockSection(5, 6, 400);
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 3);
                m_SingleBlockSect++;
                m_PreviousSect = 3;
                break;
            case 4: //one platform
                SpawnTopPlatform(3);
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 4);
                m_singlePlat++;
                m_PreviousSect = 4;
                break;
            case 5: //small platforms
                SpawnTopPlatform(3);
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 5);
                m_SmallPlatSect++;
                m_PreviousSect = 5;
                break;
            case 6: //two large platforms
                SpawnTopPlatform(6);
                m_loc += 10;
                LevelSeq += FString::Printf(TEXT("%d,"), 6);
                m_LargePlatSect++;
                m_PreviousSect = 6;
                break;
            // Ensure there's a default case, possibly to handle unexpected values
            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid section selection"));
                break;
            }
        }
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
				SpawnBlockSection(3, 6, 100);
				m_loc += 10;
				break;
			case '3':
				SpawnBlockSection(5, 6, 400);
				m_loc += 10;
				break;
			case '4':
				SpawnPlatform(4, 5);
				m_loc += 10;
				break;
			case '5':
				SpawnTopPlatform(3);
				m_loc += 10;
			case '6':
				SpawnTopPlatform(6);
				m_loc += 10;
			
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *LevelSeq);
	}
	
	Fitness();
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
	int32 randint = FMath::RandRange(0, 100);
	if(randint >=80)
	{
		SpawnUnder();
	}
}



void APCG_FitnessFunc::SpawnBlockSection(int BlockType, int length, int zAxis)
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + length) * 100, 0,zAxis); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[BlockType], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_FitnessFunc::SpawnPlatform(int BlockType, int length)
{
	SpawnEmptySection();
	for (int32 X = m_loc; X < m_loc + 3; X++)
	{
		FVector SpawnLocation = FVector((X + length) * 100, 0,400); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[BlockType], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
	
}

void APCG_FitnessFunc::SpawnTopPlatform(int length)
{
	SpawnEmptySection();
	SpawnPlatform(4, 5);
	for (int32 X = m_loc; X < m_loc + length; X++)
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
	int32 ChunkSize = 32; // Size of each small sequence //11

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

void APCG_FitnessFunc::SpawnUnder()
{
	FVector SpawnLocation = FVector(m_loc * 100, 0,-800); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[8], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

int32  APCG_FitnessFunc::SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities)
{
	// Calculate the total sum of all probabilities
	float TotalProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		TotalProbability += Elem.Value;
	}

	// Generate a random number in the range [0, TotalProbability]
	float RandomPoint = FMath::FRandRange(0.0f, TotalProbability);

	// Convert the probabilities to a cumulative distribution
	float CumulativeProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		CumulativeProbability += Elem.Value;

		// Check if the random point falls within the current cumulative probability
		if (RandomPoint <= CumulativeProbability)
		{
			return Elem.Key; // Return the section type
		}
	}

	// In case of rounding errors or empty map, return a default value or handle the error
	// Assuming 0 is a valid default
	return 0;
}

void APCG_FitnessFunc::DetermineProbability()
{
	// Define base probabilities for each section type
	SectionProbabilities = {
		{0, 10.0f}, // Empty
		{1, 10.0f}, // Pipes
		{2, 10.0f}, // Stairs
		{3, 10.0f}, // Single Block
		{4, 10.0f}, // One Platform
		{5, 10.0f}, // Small Platforms
		{6, 10.0f}  // Large Platforms
	};

	// Adjust probabilities based on the previously spawned section
	// For example, after spawning a 'large platform' section, 
	// you might want to increase the chance of spawning a 'stairs' section.
	switch(m_PreviousSect)
	{
	case 0: // Last section was Empty
		SectionProbabilities[1] += 5.0f; // Slightly increase probability of Pipes
		SectionProbabilities[2] += 5.0f; // Increase probability of Stairs to encourage verticality
		break;
	case 1: // Last section was Pipes
		SectionProbabilities[0] += 5.0f; // Increase probability of Empty for a breather
		SectionProbabilities[6] += 5.0f; // Increase probability of Large Platforms for variety
		break;
	case 2: // Last section was Stairs
		SectionProbabilities[3] += 10.0f; // Significantly increase probability of Single Block for challenge
		SectionProbabilities[5] -= 5.0f; // Decrease probability of Small Platforms to avoid repetition
		break;
	case 3: // Last section was Single Block
		SectionProbabilities[4] += 5.0f; // Increase probability of One Platform for pacing
		SectionProbabilities[0] += 10.0f; // Increase probability of Empty to break up the intensity
		break;
	case 4: // Last section was One Platform
		SectionProbabilities[5] += 10.0f; // Increase probability of Small Platforms for thematic continuation
		SectionProbabilities[1] -= 5.0f; // Decrease probability of Pipes to avoid redundancy
		break;
	case 5: // Last section was Small Platforms
		SectionProbabilities[6] += 15.0f; // Significantly increase probability of Large Platforms for a challenge
		break;
	case 6: // Last section was Large Platforms
		SectionProbabilities[2] += 10.0f; // Increase probability of Stairs to continue vertical gameplay
		SectionProbabilities[0] += 5.0f; // Increase probability of Empty for a visual and gameplay breather
		break;
	}

	// Normalize probabilities to ensure they sum to 100 (or another base value) if needed
	NormalizeProbabilities(SectionProbabilities);
}

void APCG_FitnessFunc::NormalizeProbabilities(TMap<int32, float>& Probabilities)
{
	float Total = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		Total += Elem.Value;
	}

	for (auto& Elem : Probabilities)
	{
		Elem.Value = (Elem.Value / Total) * 100.0f; // Adjusting each probability to ensure the sum is 100
	}
}

void APCG_FitnessFunc::Fitness()
{
	//UE_LOG(LogTemp, Warning, TEXT("The integer is: %d"), m_emptySect);
	

	
	UE_LOG(LogTemp, Warning, TEXT("The integer is: %d"), m_Fitness);
}


// Called every frame
void APCG_FitnessFunc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

