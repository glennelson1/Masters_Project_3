// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCG_Modified.generated.h"

UCLASS()
class APCG_Modified : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCG_Modified();

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
	void SpawnStairsSection();
	void SpawnSingleBlockSection();
	void SpawnSinglePlatform();
	void SpawnTwoPlatform();
	void SpawnTwoLargePlatform();

	int m_loc;
	int num;
	FString LevelSeq;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
