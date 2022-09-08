// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ADSComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/ADS_Interface.h"
#include "Kismet/KismetMathLibrary.h"

UADSComponent::UADSComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UADSComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UADSComponent::StartADS(AActor* UPARAM(DisplayName="MoveToActor") Actor, EMovementType MoveStatus)
{
	if(!Actor || !GetWorld()) return;
	ActorToMove = Actor;
	if(GetWorld()->GetTimerManager().IsTimerActive(TimerADS))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerADS);
	}
	if(MoveStatus == EMovementType::MoveCameraIn)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::StartToMoveCameraIn,0.075f,false);
	}
	else if(MoveStatus == EMovementType::MoveCameraOut)
	{
		StartToMoveCameraOut();
	}
}

// Functions For Timers
void UADSComponent::StartToMoveCameraIn()
{
	if(!GetWorld()) return;
	bADS = true;
	GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveIn,0.0075f,true);
}

void UADSComponent::StartToMoveCameraOut()
{
	if(!GetWorld()) return;
	bADS = false;
	GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveOut,0.0075f,true);
}

void UADSComponent::MoveIn()
{
	if(!ActorToMove || !Player || !Camera) return;
	IADS_Interface* MoveInterface = Cast<IADS_Interface>(ActorToMove);
	if(!MoveInterface) return;

	USceneComponent* LocalSceneComponent;
	FName LocalName;
	MoveInterface->Execute_GetMoveInfo(ActorToMove,LocalSceneComponent,LocalName);
	if(!UKismetMathLibrary::EqualEqual_VectorVector(Camera->GetComponentLocation(),LocalSceneComponent->GetSocketLocation(LocalName),0.01f))
	{
		Camera->SetWorldLocation(UKismetMathLibrary::VLerp(Player->GetMesh()->GetSocketLocation(Camera->GetAttachSocketName()),LocalSceneComponent->GetSocketLocation(LocalName));
	}
}

void UADSComponent::MoveOut()
{
	
}
