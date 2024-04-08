// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCG_FitnessFunc.generated.h"

UCLASS()
class APCG_FitnessFunc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCG_FitnessFunc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool RandomGen;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> CellClasses;
	UFUNCTION(BlueprintCallable)
	void DeleteGrid();
	TArray<AActor*> Cellref;
	UFUNCTION(BlueprintCallable)
	void SpawnGrid();
	
	void SpawnEmptySection();
	void SpawnPipeSection();
	void SpawnBlockSection(int BlockType, int length, int xAxis);
	void SpawnPlatform(int BlockType, int length);
	void SpawnTopPlatform(int length);
	UFUNCTION(BlueprintCallable)
	void SaveLevelSeqToFile();

	void SpawnUnder();
	int32  SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities);
	void DetermineProbability();
	void NormalizeProbabilities(TMap<int32, float>& Probabilities);
	int m_loc;
	TMap<int32, float> SectionProbabilities;

	void Fitness();
	int m_emptySect, m_pipeSect, m_StairsSect, m_SingleBlockSect,m_singlePlat, m_SmallPlatSect, m_LargePlatSect;
	int m_Fitness;
	
	
	FString LevelSeq;
    int m_PreviousSect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
