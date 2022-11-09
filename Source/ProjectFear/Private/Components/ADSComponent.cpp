// Project Fear. All rights reserved.


#include "Components/ADSComponent.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UADSComponent::UADSComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UADSComponent::StartADS(USceneComponent* SceneComponentToMove, FName SocketName)
{
	if(!SceneComponentToMove || !GetWorld() || SocketName.IsNone())
	{
		ClearMoveInfo();
		return;
	}
	MoveToComponent = SceneComponentToMove;
	NameToMove = SocketName;
	GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::StartToMoveCameraIn,0.075f,false);
}

void UADSComponent::StopADS()
{
	StartToMoveCameraOut();
}

// Set Info
void UADSComponent::SetCameraProperties(FVector CameraLocation, float CameraFOV) const
{
	UCameraComponent* Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if(!Camera) return;
	Camera->SetWorldLocation(CameraLocation);
	Camera->SetFieldOfView(CameraFOV);
}

void UADSComponent::ClearMoveInfo()
{
	MoveToComponent = nullptr;
	NameToMove = FName();
}

// Functions For Timers
void UADSComponent::StartToMoveCameraIn()
{
	bADS = true;
	ToggleADS(EMovementType::MoveCameraIn);
}

void UADSComponent::StartToMoveCameraOut()
{
	bADS = false;
	ClearMoveInfo();
	ToggleADS(EMovementType::MoveCameraOut);
}

void UADSComponent::MoveIn()
{
	UCameraComponent* Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if(!MoveToComponent || !Camera || NameToMove.IsNone() || !Camera->GetAttachParent())
	{
		CurrentMoveStatus = EMoveStatus::None;
		return;
	}
	const auto CameraAttachParent = Camera->GetAttachParent(); 
	if(!UKismetMathLibrary::EqualEqual_VectorVector(Camera->GetComponentLocation(),MoveToComponent->GetSocketLocation(NameToMove),0.0001f))
	{
		SetCameraProperties(UKismetMathLibrary::VLerp(CameraAttachParent->GetSocketLocation(Camera->GetAttachSocketName()),
			MoveToComponent->GetSocketLocation(NameToMove),
			ADS_Curve.GetRichCurve()->Eval(ADSTimer)),
			UKismetMathLibrary::Lerp(90.0f,70.0f,FOV_Curve.GetRichCurve()->Eval(ADSTimer)));
		ADSTimer = (ADSTimer + 0.035f) < 1.0f ? ADSTimer + 0.035f : 1.0f;
	}
}

void UADSComponent::MoveOut()
{
	UCameraComponent* Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if(!Camera || !Camera->GetAttachParent())
	{
		CurrentMoveStatus = EMoveStatus::None;
		return;
	}
	const auto CameraAttachParent = Camera->GetAttachParent();
	if(!UKismetMathLibrary::EqualEqual_VectorVector(Camera->GetComponentLocation(),CameraAttachParent->GetSocketLocation(Camera->GetAttachSocketName()),0.0001f))
	{
		SetCameraProperties(UKismetMathLibrary::VLerp(CameraAttachParent->GetSocketLocation(Camera->GetAttachSocketName()),
			Camera->GetComponentLocation(),
			ADS_Curve.GetRichCurve()->Eval(ADSTimer)),
			UKismetMathLibrary::Lerp(90.0f,70.0f,FOV_Curve.GetRichCurve()->Eval(ADSTimer)));
		ADSTimer = (ADSTimer - 0.045f) > 0.0f ? ADSTimer - 0.045f : 0.0f;
	}
	else
	{
		if(!GetWorld() || bADS)
		{
			Camera->SetFieldOfView(90.0f);
			Camera->SetRelativeLocation(FVector::ZeroVector);
			return;
		}
		GetWorld()->GetTimerManager().ClearTimer(TimerADS);
		ADSTimer = 0.0f;
		Camera->SetFieldOfView(90.0f);
		Camera->SetRelativeLocation(FVector::ZeroVector);
		CurrentMoveStatus = EMoveStatus::None;
	}
}

void UADSComponent::ToggleADS(EMovementType MoveStatus)
{
	if(!GetWorld()) return;
	switch (MoveStatus) 
	{
	case EMovementType::MoveCameraIn:
		if(CurrentMoveStatus != EMoveStatus::MovingIn)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveIn,0.0075f,true);
			CurrentMoveStatus = EMoveStatus::MovingIn;
		}
		break;
	case EMovementType::MoveCameraOut:
		if(CurrentMoveStatus != EMoveStatus::MovingOut)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveOut,0.0075f,true);
			CurrentMoveStatus = EMoveStatus::MovingOut;
		}
		break;
	default:
		GetWorld()->GetTimerManager().ClearTimer(TimerADS);
		CurrentMoveStatus = EMoveStatus::None;
	}
}

// Debug Logic
FString UADSComponent::ADSInfo() const
{
	FString DebugStatus;
	switch (CurrentMoveStatus)
	{
		case EMoveStatus::MovingIn:
			DebugStatus = "MovingIn";
			break;
		case EMoveStatus::MovingOut:
			DebugStatus = "MovingOut";
			break;
		case EMoveStatus::None:
			DebugStatus = "None";
			break;
		default:
			break;
	}
	FString DebugInfo = "Is ADS: " + FString(bADS ? "True" : "False")+ "\n"
	+ "Move To Component: " + FString(MoveToComponent ? "Valid" : "Not Valid") + "\n"
	+ "Socket Name: " + NameToMove.ToString() + "\n"
	+ DebugStatus + "\n";
	return DebugInfo;
}
