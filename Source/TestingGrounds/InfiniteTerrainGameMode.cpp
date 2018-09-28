// Ryu
#include "InfiniteTerrainGameMode.h"
#include "GameFramework/Volume.h" //??
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() {
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Navigation Mesh Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator) {
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}


void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumeToAdd) {
	NavMeshBoundsVolumePool->Add(VolumeToAdd);

	//auto VolumeName = VolumeToAdd->GetName();
	//if (VolumeName.Contains(FString("NavMesh"), ESearchCase::CaseSensitive, ESearchDir::FromStart))
	//	UE_LOG(LogTemp, Warning, TEXT("Found NavMesh: %s"), *VolumeToAdd->GetName());
}