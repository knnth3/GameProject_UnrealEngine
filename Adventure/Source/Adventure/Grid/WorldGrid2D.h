// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include <map>
#include <memory>
#include <vector>

#include "Basics.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGrid2D.generated.h"

typedef std::pair<int, int> coordinate;

struct ADVENTURE_API GridCell
{
	GridCell();
	GridCell(FGridCoordinate location);
	int F_Cost()const;
	bool operator<(const GridCell& b);
	std::list<std::shared_ptr<GridCell>> GetEmptyNeighbors();

	//Pathfinding
	FGridCoordinate Location;
	bool Ocupied;
	int H_Cost;
	int G_Cost;
	std::shared_ptr<GridCell> Parent;

	//Direct
	std::shared_ptr<GridCell> Top;
	std::shared_ptr<GridCell> Left;
	std::shared_ptr<GridCell> Right;
	std::shared_ptr<GridCell> Bottom;

	//Diagonal
	std::shared_ptr<GridCell> TopLeft;
	std::shared_ptr<GridCell> BottomLeft;
	std::shared_ptr<GridCell> TopRight;
	std::shared_ptr<GridCell> BottomRight;
};

typedef std::shared_ptr<GridCell> GridCellPtr;

//Indexing starts at (0,0)
UCLASS()
class ADVENTURE_API AWorldGrid2D : public AActor
{

	GENERATED_BODY()

public:

	AWorldGrid2D();
	GridCellPtr At(FGridCoordinate Location)const;
	int GetDistance(FGridCoordinate Begin, FGridCoordinate End)const;
	int GetDistance(const GridCellPtr& Begin, const GridCellPtr& End)const;

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	void MakeGrid(int Rows, int Columns);

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	bool FindPath(FGridCoordinate Begin, FGridCoordinate End, TArray<FGridCoordinate>& list);

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	void Clear();

	//Order of spawn locations will not matter
	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	void SetSpawnLocations(const TArray<FGridCoordinate>& locations);

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	void RemoveSpawnLocation(FGridCoordinate Location);

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	void SetOccupied(FGridCoordinate Location);

	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	bool IsOccupied(FGridCoordinate Location)const;

	//False implies all spawn points are occupied
	UFUNCTION(BlueprintCallable, Category = "Grid2D")
	bool GetOpenSpawnLocation(FGridCoordinate& GridLocation);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	UFUNCTION(BlueprintImplementableEvent, Category = "Map Pawn")
	void OnGridResize(int rows, int columns);

private:

	UFUNCTION(NetMulticast, reliable)
	void Multicast_MakeGrid(int rows, int columns);

	void CreateStaticMesh();

	UPROPERTY(Replicated)
	int m_gridRows;

	UPROPERTY(Replicated)
	int m_gridColumns;

	vector2D<GridCellPtr>        m_grid;
	std::vector<FGridCoordinate> m_spawnLocations;
};

class ADVENTURE_API AstarPathFind
{
public:
	bool FindPath(AWorldGrid2D* grid, FGridCoordinate Begin, FGridCoordinate End, std::list<FGridCoordinate>& list);
	std::list<FGridCoordinate> TraceParentOwnership(const GridCellPtr& begin, const GridCellPtr& end);

private:
	coordinate GetCoordinate(const FGridCoordinate& Location)const;
};