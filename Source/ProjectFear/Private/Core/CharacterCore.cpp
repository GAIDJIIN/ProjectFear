// Project Fear. All rights reserved.

#include "Core/CharacterCore.h"

#include "Camera/CameraComponent.h"
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
	if(!GetWorld() || !GetComponentByClass(UCameraComponent::StaticClass())) return;
	const auto Camera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
	const FVector StartTrace = GetMesh()->GetSocketLocation(Camera->GetAttachSocketName());
	const FRotator ViewRotation = Camera->GetComponentRotation();
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
			if(InteractableActor && InteractableActor != LocalInterface) OnLostInteract();
			OnFindInteract(LocalInterface);
		}
		else if(InteractableActor) OnLostInteract();
	}
	else if(InteractableActor) OnLostInteract();
}

void ACharacterCore::OnFindInteract(IInteractInterface* InteractActor)
{
	InteractActor->StartHover();
	InteractableActor = InteractActor;
}

void ACharacterCore::OnLostInteract()
{
	InteractableActor->StopHover();
	StopInteract();
	InteractableActor = nullptr;
}

void ACharacterCore::TryInteract()
{
	if(InteractableActor) InteractableActor->StartInteract(this);
}

void ACharacterCore::StopInteract()
{
	if(InteractableActor) InteractableActor->StopInteract();
}

