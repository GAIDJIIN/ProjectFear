// Project Fear. All rights reserved.


#include "Items/PickUpCore.h"

APickUpCore::APickUpCore()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APickUpCore::BeginPlay()
{
	Super::BeginPlay();
}

// Interact Logic
void APickUpCore::StartInteract(const ACharacter* Player)
{
	if(Player) StartInteractLogic(Player);
}

void APickUpCore::StopInteract()
{
	StopInteractLogic();
}

// Interact Hovered Logic
void APickUpCore::StartHover()
{
	StartHoverLogic();
}

void APickUpCore::StopHover()
{
	StopHoverLogic();
}

