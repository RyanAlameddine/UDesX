// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VoxelActor.generated.h"

UCLASS()
class UDESX_API AVoxelActor : public AActor
{
	GENERATED_BODY()

public:
		//material count = voxel count
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 randomSeed = 0;

	//size of each voxel in UE units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 voxelSize = 100;
	
	//chunk size in blocks (base of chunk is chunkWidth by chunkWidth)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkWidth = 10; //chunkLineElements

	//in chunk coords
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkX = 0; 

	//in chunk coords
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkY = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float xMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float yMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float zMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float weight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float frequency = 1;

	UPROPERTY()
		int32 totalBlockCount; //chunkTotalElements

	UPROPERTY()
		int32 chunkBlockHeight; //chunkZElements

	UPROPERTY()
		int32 chunkWidthSquared; //chunkLineElementsSquared

	UPROPERTY()
		int32 voxelSizeHalf;

	UPROPERTY()
		TArray<int32> blocks; //chunkFields

	UPROPERTY()
		UProceduralMeshComponent* proceduralComponent;

public:
	// Sets default values for this actor's properties
	AVoxelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateChunk();

	void UpdateMesh();
};
