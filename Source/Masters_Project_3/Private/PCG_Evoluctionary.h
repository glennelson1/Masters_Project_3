// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCG_Evoluctionary.generated.h"

UCLASS()
class APCG_Evoluctionary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCG_Evoluctionary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<FString> Population;
	TArray<float> FitnessScores;
	
	float m_MutationRate;
	int m_CurrentGeneration;
	int m_MaxGenerations = 100;
	float m_DiversityThreshold = 0.2f;
	int m_EliteCount;
	
	void InitializePopulation();
	void EvaluatePopulation();
	void Selection();
	void PreserveElites(TArray<FString>& NewPopulation);
	void Crossover();
	float CalculateDiversity();
	void AdjustMutationRate();
	void Mutation();
	float CalculateFitness(const FString& LevelSequence);

	
	
	
	void SpawnGridFromGenome(const FString& Genome);

	

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> CellClasses;
	UFUNCTION(BlueprintCallable)
	void DeleteGrid();
	TArray<AActor*> Cellref;

	
	int m_loc;
	void SpawnEmptySection();
	void SpawnPipeSection();
	void SpawnBlockSection(int BlockType, int length, int xAxis);
	void SpawnPlatform(int BlockType, int length);
	void SpawnTopPlatform(int length);
	void SpawnUnder();
	void SaveData(const FString& Genome, const FString& Data);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
