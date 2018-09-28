// Ryu
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition {
	GENERATED_USTRUCT_BODY()
	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor {
	GENERATED_BODY()
	
public:	
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceActors(
		TSubclassOf<AActor> ToSpawn,
		int MinSpawn = 1,
		int MaxSpawn = 1,
		float Radius = 500,
		float MinScale = 1,
		float MaxScale = 1
	);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(
		TSubclassOf<APawn> ToSpawn,
		int MinSpawn = 1,
		int MaxSpawn = 1,
		float Radius = 500
	);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

private:

	void PositionNavMeshBoundsVolume();

	TArray<FSpawnPosition> RandomSpawnPositions(
		int MinSpawn,
		int MaxSpawn,
		float Radius,
		float MinScale,
		float MaxScale
	);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
	void PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);
	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

};
