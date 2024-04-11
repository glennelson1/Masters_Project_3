// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_Evoluctionary.h"
APCG_Evoluctionary::APCG_Evoluctionary()
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
void APCG_Evoluctionary::BeginPlay()
{
	Super::BeginPlay();
	
}
void APCG_Evoluctionary::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	m_PreviousSect = 0;
	m_Fitness = 0;
	LevelSeq.Empty();
}
void APCG_Evoluctionary::SpawnGrid()
{
	AdjustProbabilitiesBasedOnSequencesFromFile();

	DeleteGrid();
	m_loc = 0; 

	
	for (int i = 0; i <= 20; i++) // Generate 20 sections, for example
		{
		
			int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
		
			switch(SelectedSection)
			{
			case 0: SpawnEmptySection(); m_emptySect++; break;
			case 1: SpawnPipeSection(); m_pipeSect++; break;
			case 2: SpawnBlockSection(3, 6, 100); m_StairsSect++; break;
			case 3: SpawnBlockSection(5, 6, 400); m_SingleBlockSect++; break;
			case 4: SpawnTopPlatform(3); m_singlePlat++; break;
			case 5: SpawnTopPlatform(3); m_SmallPlatSect++; break;
			case 6: SpawnTopPlatform(6); m_LargePlatSect++; break;
			default: UE_LOG(LogTemp, Warning, TEXT("Invalid section selection")); break;
			}
			m_loc += 10; // Assuming each section advances 'm_loc' by 10 units
			LevelSeq += FString::Printf(TEXT("%d,"), SelectedSection); // Append to the level sequence
			m_PreviousSect = SelectedSection; // Update the previous section for next iteration
		}
}


void APCG_Evoluctionary::SpawnEmptySection()
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

void APCG_Evoluctionary::SpawnPipeSection()
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

void APCG_Evoluctionary::SpawnBlockSection(int BlockType, int length, int zAxis)
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + length) * 100, 0,zAxis); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[BlockType], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_Evoluctionary::SpawnPlatform(int BlockType, int length)
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

void APCG_Evoluctionary::SpawnTopPlatform(int length)
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

void APCG_Evoluctionary::SpawnUnder()
{
	FVector SpawnLocation = FVector(m_loc * 100, 0,-800); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[8], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void APCG_Evoluctionary::SaveLevelSeqToFile()
{
	FString SaveFilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/AllGoodLevelSeqs.txt");
	int32 ChunkSize = 50;
	
	FString SaveDirectory = FPaths::GetPath(SaveFilePath);
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.DirectoryExists(*SaveDirectory))
	{
		FileManager.MakeDirectory(*SaveDirectory, true);
	}


	for (int32 StartIndex = 0; StartIndex < LevelSeq.Len(); StartIndex += ChunkSize)
	{
		
		FString SubSequence = LevelSeq.Mid(StartIndex, ChunkSize);
        
		
		FString ContentToSave = SubSequence + TEXT("\n");

		
		bool bWasSuccessful = FFileHelper::SaveStringToFile(ContentToSave, *SaveFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

		
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


int32 APCG_Evoluctionary::SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities)
{
	float TotalProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		TotalProbability += Elem.Value;
	}

	
	float RandomPoint = FMath::FRandRange(0.0f, TotalProbability);

	
	float CumulativeProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		CumulativeProbability += Elem.Value;

		
		if (RandomPoint <= CumulativeProbability)
		{
			return Elem.Key; // Return the section type
		}
	}

	return 0;
}




void APCG_Evoluctionary::ResetProbabilitiesToDefault()
{
	SectionProbabilities = {
		{0, 10.0f}, // Empty
		{1, 10.0f}, // Pipes
		{2, 10.0f}, // Stairs
		{3, 10.0f}, // Single Block
		{4, 10.0f}, // One Platform
		{5, 0.0f}, // Small Platforms
		{6, 0.0f}  // Large Platforms
	};
}

void APCG_Evoluctionary::AdjustProbabilitiesBasedOnSequencesFromFile()
{
	FString FilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/AllGoodLevelSeqs.txt");
    TArray<FString> PastSequences;
    
    if (FFileHelper::LoadFileToStringArray(PastSequences, *FilePath))
    {
        FString SequencesLog = TEXT("Loaded Past Sequences:\n");
        for (const FString& Sequence : PastSequences)
        {
            SequencesLog += Sequence + TEXT("\n");
        }
        //UE_LOG(LogTemp, Log, TEXT("%s"), *SequencesLog);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load sequences from %s"), *FilePath);
        return;
    }

    TMap<FString, int32> SequenceCounts;
    for (const FString& Sequence : PastSequences)
    {
        if (SequenceCounts.Contains(Sequence))
        {
            SequenceCounts[Sequence]++;
        }
        else
        {
            SequenceCounts.Add(Sequence, 1);
        }
    }

    FString MostCommonSequence;
    int32 HighestCount = 0;
    for (const auto& Elem : SequenceCounts)
    {
        if (Elem.Value > HighestCount)
        {
            HighestCount = Elem.Value;
            MostCommonSequence = Elem.Key;
        }
    }

    ResetProbabilitiesToDefault();

    // Analyze the most common sequence for detailed adjustments
    if (MostCommonSequence.Contains("1")) // If pipes are common
    {
        // Adjustments based on pipes being common
        SectionProbabilities[2] += 5.0f; // Increase for stairs
        SectionProbabilities[1] -= 5.0f; // Decrease for pipes
    	UE_LOG(LogTemp, Log, TEXT("1"));
    }

    // Further adjustments based on analysis
    // This is simplistic; for more nuanced game design, consider more complex patterns and adjustments
    if (MostCommonSequence.Contains("3")) // If single blocks are common
    {
    	UE_LOG(LogTemp, Log, TEXT("2"));
        SectionProbabilities[4] += 5.0f; // Increase for one platform
        SectionProbabilities[3] -= 5.0f; // Decrease for single block
    	
    }
	if (MostCommonSequence.Contains("3")) // If single blocks are common
		{
		UE_LOG(LogTemp, Log, TEXT("2"));
		SectionProbabilities[4] += 5.0f; // Increase for one platform
		SectionProbabilities[3] -= 5.0f; // Decrease for single block
    	
		}
    if (MostCommonSequence.Contains("5,6")) // Example: small and large platforms sequence
    {
    	UE_LOG(LogTemp, Log, TEXT("3"));
        SectionProbabilities[0] += 10.0f; // Significantly increase empty spaces for a break
        SectionProbabilities[5] -= 5.0f; // Decrease small platforms
        SectionProbabilities[6] -= 5.0f; // Decrease large platforms
    }

    NormalizeProbabilities(SectionProbabilities);
}
void APCG_Evoluctionary::IdentifyPatternsInSequences(const TArray<FString>& Sequences,TMap<FString, int32>& PatternCounts)
{
	for (const FString& Sequence : Sequences)
	{
		for (int32 i = 0; i < Sequence.Len() - 1; ++i) // Adjust -1 based on pattern length
			{
			FString Pattern = Sequence.Mid(i, 2); // Adjust 2 based on pattern length
			if (PatternCounts.Contains(Pattern))
			{
				PatternCounts[Pattern]++;
			}
			else
			{
				PatternCounts.Add(Pattern, 1);
			}
			}
	}
}

void APCG_Evoluctionary::AdjustProbabilitiesBasedOnPatterns(const FString& MostCommonPattern)
{
	if (MostCommonPattern.Equals("12")) // Assuming 1 is pipe and 2 is stairs
		{
		SectionProbabilities[0] += 10.0f; // Increase empty
		SectionProbabilities[1] -= 5.0f;  // Decrease pipes
		SectionProbabilities[2] -= 5.0f;  // Decrease stairs
		}
}
void APCG_Evoluctionary::NormalizeProbabilities(TMap<int32, float>& Probabilities)
{
	float Total = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		Total += Elem.Value;
	}

	for (auto& Elem : Probabilities)
	{
		Elem.Value = (Elem.Value / Total) * 100.0f; 
	}
	
}
// Called every frame
void APCG_Evoluctionary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

