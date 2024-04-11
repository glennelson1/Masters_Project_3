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
	
	void NormalizeProbabilities(TMap<int32, float>& Probabilities);

	
	int m_loc;
	TMap<int32, float> SectionProbabilities;

	
	int m_emptySect, m_pipeSect, m_StairsSect, m_SingleBlockSect,m_singlePlat, m_SmallPlatSect, m_LargePlatSect;
	int m_Fitness;
	TArray<FString> LoadFrequentPatterns();
	int32  SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities);
	void IdentifyPatternsInSequences(const TArray<FString>& Sequences, TMap<FString, int32>& PatternCounts);
	void AdjustProbabilitiesBasedOnPatterns(const FString& MostCommonPattern);
	void SaveFrequentPatterns(const TMap<FString, int32>& PatternCounts);
	void ResetProbabilitiesToDefault();
	void AdjustProbabilitiesBasedOnSequencesFromFile();
	FString LevelSeq;
	int m_PreviousSect;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
