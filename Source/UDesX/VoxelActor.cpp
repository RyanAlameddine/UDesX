// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelActor.h"

const int32 bTriangles[] = { 2, 1, 0, 0, 3, 2 };
const FVector2D bUVs[] = { FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0) };
const FVector bNormals0[] = { FVector(0, 0, 1) , FVector(0, 0, 1) , FVector(0, 0, 1) , FVector(0, 0, 1) };
const FVector bNormals1[] = { FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1) };
const FVector bNormals2[] = { FVector(0, 1, 0) , FVector(0, 1, 0) , FVector(0, 1, 0) , FVector(0, 1, 0) };
const FVector bNormals3[] = { FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0) };
const FVector bNormals4[] = { FVector(1, 0, 0) , FVector(1, 0, 0) , FVector(1, 0, 0) , FVector(1, 0, 0) };
const FVector bNormals5[] = { FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0) };
const FVector bMask[] = { FVector(0, 0, 1), FVector(0, 0, -1), FVector(0, 1, 0), FVector(0, -1, 0), FVector(1, 0, 0), FVector(-1, 0, 0) };

// Sets default values
AVoxelActor::AVoxelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVoxelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVoxelActor::OnConstruction(const FTransform& Transform)
{
	chunkZElements = 80;
	totalChunkElements = chunkLineElements * chunkLineElements * chunkZElements;
	chunkLineElementsSquared = chunkLineElements * chunkLineElements;
	voxelSizeHalf = voxelSize / 2;

	FString string = "Voxel_" + FString::FromInt(chunkXIndex) + "_" + FString::FromInt(chunkYIndex);
	FName name = FName(*string);
	proceduralComponent = NewObject<UProceduralMeshComponent>(this, name);
	proceduralComponent->RegisterComponent();

	RootComponent = proceduralComponent;
	RootComponent->SetWorldTransform(Transform);

	Super::OnConstruction(Transform);

	GenerateChunk();
	UpdateMesh();
}

void AVoxelActor::GenerateChunk()
{
	chunkFields.SetNumUninitialized(totalChunkElements);

	for (int32 x = 0; x < chunkLineElements; x++) 
	{
		for (int32 y = 0; y < chunkLineElements; y++)
		{
			for (int32 z = 0; z < chunkZElements; z++)
			{
				int32 index = x + (y * chunkLineElements) + (z * chunkLineElementsSquared);

				chunkFields[index] = (z < 29) ? 1 : 0;
			}
		}
	}
}

void AVoxelActor::UpdateMesh()
{
	TArray<FVector> Verticies;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	int32 elementID = 0;
	
	for (int32 x = 0; x < chunkLineElements; x++)
	{
		for (int32 y = 0; y < chunkLineElements; y++)
		{
			for (int32 z = 0; z < chunkZElements; z++)
			{
				int32 index = x + (y * chunkLineElements) + (z * chunkLineElementsSquared);
				int32 meshIndex = chunkFields[index];

				if (meshIndex > 0) 
				{
					meshIndex--;

					//add faces, verticies, UVs, and Normals
					int triangle_num = 0;
					for (int i = 0; i < 6; i++)
					{
						FVector mask = bMask[i];
						int newIndex = index + mask.X + (mask.Y * chunkLineElements) + (mask.Z * chunkLineElementsSquared);

						bool flag = false;
						if (meshIndex >= 20) flag = true;
						else if ((x + mask.X < chunkLineElements) && (x + mask.X >= 0) && (y + mask.Y < chunkLineElements) && (y + mask.Y >= 0))
						{
							if (newIndex < chunkFields.Num() && newIndex >= 0)
							{
								if (chunkFields[newIndex] < 1) flag = true;
							}
						}
						else flag = true;

						if (flag)
						{
							Triangles.Add(bTriangles[0] + triangle_num + elementID);
							Triangles.Add(bTriangles[1] + triangle_num + elementID);
							Triangles.Add(bTriangles[2] + triangle_num + elementID);
							Triangles.Add(bTriangles[3] + triangle_num + elementID);
							Triangles.Add(bTriangles[4] + triangle_num + elementID);
							Triangles.Add(bTriangles[5] + triangle_num + elementID);
							triangle_num += 4; // add four verts to offset the next triangle

							switch (i)
							{
							case 0: {
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals0, UE_ARRAY_COUNT(bNormals0));
								break;
							}
							case 1: {
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals1, UE_ARRAY_COUNT(bNormals1));
								break;
							}
							case 2: {
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals2, UE_ARRAY_COUNT(bNormals2));
								break;
							}
							case 3: {
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector( voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals3, UE_ARRAY_COUNT(bNormals3));
								break;
							}
							case 4: {
								Verticies.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals4, UE_ARRAY_COUNT(bNormals4));
								break;
							}
							case 5: {
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize),  voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Verticies.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize),  voxelSizeHalf + (z * voxelSize)));
							
								Normals.Append(bNormals5, UE_ARRAY_COUNT(bNormals5));
								break;
							}
							}

							UVs.Append(bUVs, UE_ARRAY_COUNT(bUVs));
							//FColor color = FColor::MakeRandomColor();

							//FColor color = F...
							FColor color = FColor(255, 255, 255, i);
							VertexColors.Add(color); VertexColors.Add(color); VertexColors.Add(color); VertexColors.Add(color);
						}
					}

					//el_num += triangle_num;
					elementID += triangle_num;
				}
			}
		}
	}

	proceduralComponent->ClearAllMeshSections();

	proceduralComponent->CreateMeshSection(0, Verticies, Triangles, Normals, UVs, VertexColors, Tangents, true);
}