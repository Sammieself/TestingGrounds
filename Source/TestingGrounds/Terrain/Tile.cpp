// Ryu
#include "Tile.h"
#include "TestingGrounds.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Public/NavigationSystem.h"
#include "ActorPool.h"

ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

template<class T>
void ATile::RandomlyPlaceActors(
	TSubclassOf<T> ToSpawn,
	int MinSpawn,
	int MaxSpawn,
	float Radius,
	float MinScale,
	float MaxScale) {

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius) {
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition& SpawnPosition) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))	{
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

// Called when the game starts or when spawned
void ATile::BeginPlay() { Super::BeginPlay(); }

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	/*UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay"), *GetName())*/
	if (Pool != nullptr && NavMeshBoundsVolume != nullptr) {
		Pool->Return(NavMeshBoundsVolume);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATile::SetPool(UActorPool * InPool) {
	UE_LOG(LogTemp, Warning, TEXT("[%s] setting %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume() {
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Not enough actors in pool"))
			return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	UE_LOG(LogTemp, Warning, TEXT("Actor Location: %s"), *NavMeshBoundsVolume->GetActorLocation().ToString())
	FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->Build();
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

/*
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(
		GetWorld(),
		GlobalLocation,
		0,
		Radius,
		FQuat::Identity,
		ResultColor,
		true,
		100
	);
*/
	return !HasHit;
}