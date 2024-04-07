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
	void SpawnStairsSection();
	void SpawnSingleBlockSection();
	void SpawnSinglePlatform();
	void SpawnTwoPlatform();
	void SpawnTwoLargePlatform();
	UFUNCTION(BlueprintCallable)
	void SaveLevelSeqToFile();
	
	int m_loc;
	
	FString LevelSeq;
    int m_PreviousSect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
