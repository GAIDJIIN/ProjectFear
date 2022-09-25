// Project Fear. All rights reserved.

#include "Core/CharacterCore.h"
#include "Core/InteractInterface.h"
#include "Kismet/KismetSystemLibrary.h"

ACharacterCore::ACharacterCore()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterCore::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bCanInteract) FindInteract();
}

void ACharacterCore::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Interact Logic
void ACharacterCore::FindInteract()
{
	if(!GetWorld()) return;
	FVector StartTrace;
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(),StartTrace,25.0f);
	FRotator ViewRotation;
	const auto LocalPlayerController = GetController<APlayerController>();
	if(!LocalPlayerController) return;
	LocalPlayerController->GetPlayerViewPoint(StartTrace,ViewRotation);
	const FVector EndTrace = StartTrace + ViewRotation.Vector()*InteractTraceDistance;
	FHitResult LocalHitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		StartTrace,
		EndTrace,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{this},
		bDrawInteractDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		LocalHitResult,
		true,
		FColor::Red,
		FColor::Green,
		0.0f
		);
	DistanceToActor = LocalHitResult.bBlockingHit ? (LocalHitResult.ImpactPoint - StartTrace).Length() : (EndTrace - StartTrace).Length();
	if(LocalHitResult.bBlockingHit && LocalHitResult.GetActor())
	{
		const auto LocalInterface = Cast<IInteractInterface>(LocalHitResult.GetActor());
		if(LocalInterface)
		{
			LocalInterface->StartHover();
			InteractableActor = LocalInterface;
		}
		else if(InteractableActor)
		{
			InteractableActor->StopHover();
			StopInteract();
			InteractableActor = nullptr;
		}
	}
	else if(InteractableActor)
	{
		InteractableActor->StopHover();
		StopInteract();
		InteractableActor = nullptr;
	}
}

void ACharacterCore::TryInteract()
{
	if(InteractableActor) InteractableActor->StartInteract(this);
}

void ACharacterCore::StopInteract()
{
	InteractableActor->StopInteract();
}

