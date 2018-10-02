// Ryu
#include "Mannequin.h"
#include "TestingGrounds.h"
#include "../Weapons/BallGun.h"
#include "Engine.h"

// Sets default values
AMannequin::AMannequin() {

	PrimaryActorTick.bCanEverTick = true;
	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 20.0f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -170.0f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay() {
	Super::BeginPlay();
	if (BallGunBlueprint == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint is missing."));
		return;
	}

	BallGun = GetWorld()->SpawnActor<ABallGun>(BallGunBlueprint);

	//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	if (IsPlayerControlled()) {
		BallGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("FPGripPoint"));
	} else {
		BallGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("TPGripPoint"));
	}

	BallGun->AnimInstance1P = Mesh1P->GetAnimInstance();
	BallGun->AnimInstance3P = GetMesh()->GetAnimInstance();

	if (InputComponent != nullptr) {
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMannequin::UnPossessed() {
	Super::UnPossessed();
	BallGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("TPGripPoint"));
}

void AMannequin::PullTrigger() {
	if (BallGun != nullptr) {
		BallGun->OnFire();
	}
}
