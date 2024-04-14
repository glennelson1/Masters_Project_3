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

	void InitializePopulation();
	void EvaluatePopulation();
	void Selection();
	void Crossover();
	void Mutation();
	float CalculateFitness(const FString& LevelSequence);

	
	UFUNCTION(BlueprintCallable)
	void SpawnLevel();
	
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
