// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG_Evoluctionary.h"


APCG_Evoluctionary::APCG_Evoluctionary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitializePopulation();
	m_MutationRate = 0.1f;  
	m_CurrentGeneration = 0;
	m_EliteCount = 2; 
}

// Called when the game starts or when spawned
void APCG_Evoluctionary::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCG_Evoluctionary::InitializePopulation()
{
	int PopulationSize = 10;  // Size of the population
	int GenomeLength = 20;    // Number of sections in each level sequence

	for (int i = 0; i < PopulationSize; i++) {
		FString Genome;
		for (int j = 0; j < GenomeLength; j++) {
			int SectionType = FMath::RandRange(0, 6);  // Assuming section types range from 0 to 6
			Genome += FString::FromInt(SectionType);
			if (j < GenomeLength - 1) Genome += ",";
		}
		Population.Add(Genome);
	}
}

void APCG_Evoluctionary::EvaluatePopulation()
{
	FitnessScores.Empty();
	for (const FString& Genome : Population)
	{
		float Fitness = CalculateFitness(Genome);
		FitnessScores.Add(Fitness);
	}
}

void APCG_Evoluctionary::Selection()
{
	TArray<FString> NewPopulation;

	
	PreserveElites(NewPopulation);

	// Perform tournament selection for the rest of the new population slots
	int TournamentSize = 3;
	while (NewPopulation.Num() < Population.Num()) {
		FString BestGenome;
		float BestFitness = -1;
		for (int j = 0; j < TournamentSize; j++) {
			int RandomIndex = FMath::RandRange(0, Population.Num() - 1);
			if (FitnessScores[RandomIndex] > BestFitness) {
				BestFitness = FitnessScores[RandomIndex];
				BestGenome = Population[RandomIndex];
			}
		}
		NewPopulation.Add(BestGenome);
	}

	Population = NewPopulation;
}

void APCG_Evoluctionary::PreserveElites(TArray<FString>& NewPopulation)
{
	TArray<int> IndexesOfElites;

	// Find the indexes of the top elites based on their fitness scores
	for (int i = 0; i < m_EliteCount; ++i) {
		float MaxFitness = -1;
		int IndexOfMax = -1;

		for (int j = 0; j < FitnessScores.Num(); ++j) {
			if (!IndexesOfElites.Contains(j) && FitnessScores[j] > MaxFitness) {
				MaxFitness = FitnessScores[j];
				IndexOfMax = j;
			}
		}

		if (IndexOfMax != -1) {
			IndexesOfElites.Add(IndexOfMax);
		}
	}

	// Add elites to the new population directly
	for (int Index : IndexesOfElites) {
		NewPopulation.Add(Population[Index]);
	}
}


void APCG_Evoluctionary::Crossover()
{
	TArray<FString> NewPopulation;
	for (int i = 0; i < Population.Num(); i += 2) {
		FString Parent1 = Population[i];
		FString Parent2 = Population[i + 1]; // Make sure population size is even

		// Single point crossover
		TArray<FString> Sections1, Sections2;
		Parent1.ParseIntoArray(Sections1, TEXT(","));
		Parent2.ParseIntoArray(Sections2, TEXT(","));
		int CrossoverPoint = FMath::RandRange(1, Sections1.Num() - 2);

		for (int j = CrossoverPoint; j < Sections1.Num(); j++) {
			FString Temp = Sections1[j];
			Sections1[j] = Sections2[j];
			Sections2[j] = Temp;
		}

		NewPopulation.Add(FString::Join(Sections1, TEXT(",")));
		NewPopulation.Add(FString::Join(Sections2, TEXT(",")));
	}

	Population = NewPopulation;
}

float APCG_Evoluctionary::CalculateDiversity()
{
	TSet<FString> uniqueGenomes;
	for (const FString& genome : Population) {
		uniqueGenomes.Add(genome);
	}
	return static_cast<float>(uniqueGenomes.Num()) / Population.Num(); 
}

void APCG_Evoluctionary::AdjustMutationRate()
{
	// Reduce the mutation rate as the number of generations increases
	float decayFactor = 1.0f - static_cast<float>(m_CurrentGeneration) / m_CurrentGeneration;
	m_MutationRate = 0.1f * decayFactor;

	// Calculate current diversity
	float currentDiversity = CalculateDiversity();

	// If diversity is too low, boost the mutation rate
	if (currentDiversity < m_DiversityThreshold) {
		m_MutationRate *= 1.5f;  // Increase mutation rate by 50%
	}

	// Ensure the mutation rate doesn't exceed original or drop too low
	m_MutationRate = FMath::Clamp(m_MutationRate, 0.01f, 0.1f);

	
	m_CurrentGeneration++;
}

void APCG_Evoluctionary::Mutation()
{
	for (FString& Genome : Population) {
		TArray<FString> Sections;
		Genome.ParseIntoArray(Sections, TEXT(","));
		for (int i = 0; i < Sections.Num(); i++) {
			if (FMath::FRand() < m_MutationRate) { 
				Sections[i] = FString::FromInt(FMath::RandRange(0, 6));
			}
		}
		Genome = FString::Join(Sections, TEXT(","));
	}
}



float APCG_Evoluctionary::CalculateFitness(const FString& LevelSequence)
{
	TArray<FString> Sections;
	LevelSequence.ParseIntoArray(Sections, TEXT(","), true);
	TSet<FString> UniqueSections(Sections);
	return static_cast<float>(UniqueSections.Num()); 
}

void APCG_Evoluctionary::SpawnLevel()
{
	EvaluatePopulation();
	Selection();
	Crossover();
	Mutation();
	SpawnGridFromGenome(Population[0]);
}

void APCG_Evoluctionary::SpawnGridFromGenome(const FString& Genome)
{

	DeleteGrid();
    
	TArray<FString> Sections;
	Genome.ParseIntoArray(Sections, TEXT(","), true);
	m_loc = 0;
	TMap<int32, int32> SectionCount;

	m_loc = 0; 

	for (const FString& SectionCode : Sections) {
		int32 SectionType = FCString::Atoi(*SectionCode);
		SectionCount.FindOrAdd(SectionType)++;

		// Spawn the section based on the type code
		switch (SectionType) {
		case 0:
			SpawnEmptySection();
			break;
		case 1:
			SpawnPipeSection();
			break;
		case 2:
			SpawnBlockSection(3, 6, 100); 
			break;
		case 3:
			SpawnBlockSection(5, 6, 400); 
			break;
		case 4:
			SpawnPlatform(4, 5);
			break;
		case 5:
			SpawnTopPlatform(3);
			break;
		case 6:
			SpawnTopPlatform(6);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid section type: %s"), *SectionCode);
			break;
		}

		m_loc += 10;
	}
	FString SectionCountData;
	for (const auto& Elem : SectionCount) {
		SectionCountData += FString::Printf(TEXT("Type %d: %d, "), Elem.Key, Elem.Value);
	}
	SaveData(Genome, SectionCountData); 
	
	UE_LOG(LogTemp, Log, TEXT("Spawned level grid from genome: %s"), *Genome);
}

void APCG_Evoluctionary::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
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

void APCG_Evoluctionary::SpawnBlockSection(int BlockType, int length, int xAxis)
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + length) * 100, 0,xAxis); 
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

void APCG_Evoluctionary::SaveData(const FString& Genome, const FString& Data)
{
	FString SaveFilePath;
	// Check if it is the 100th generation
	if (m_CurrentGeneration == 100) {
		SaveFilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/GameLevelDataAfterEvo.txt");
	} else {
		SaveFilePath = FPaths::ProjectDir() + TEXT("/LevelSequences/GameLevelData.txt");
	}

	FString ContentToSave = FString::Printf(TEXT("Genome: %s | Data: %s\n"), *Genome, *Data);

	if (!FFileHelper::SaveStringToFile(ContentToSave, *SaveFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append)) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to save data to %s"), *SaveFilePath);
	}
	
}


void APCG_Evoluctionary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EvaluatePopulation();
	Selection();
	Crossover();
	AdjustMutationRate();  
	Mutation();
	SpawnGridFromGenome(Population[0]);

}

