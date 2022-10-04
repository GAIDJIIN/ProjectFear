// Project Fear. All rights reserved.


#include "Components/ADSComponent.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogADS,All,All)
UADSComponent::UADSComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UADSComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeInfo();
	check(Player)
	check(Camera)
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
void UADSComponent::InitializeInfo()
{
	ACharacter* LocalCharacter = Cast<ACharacter>(GetOwner());
	if(!LocalCharacter) return;
	Player = LocalCharacter;
	UCameraComponent* LocalCamera = Cast<UCameraComponent>(Player->GetComponentByClass(UCameraComponent::StaticClass()));
	if(!LocalCamera) return;
	Camera = LocalCamera;
}

void UADSComponent::SetCameraProperties(FVector CameraLocation, float CameraFOV) const
{
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
	if(!MoveToComponent || !Player || !Camera || NameToMove.IsNone())
	{
		CurrentMoveStatus = None;
		return;
	}
	UE_LOG(LogADS,Display,TEXT("MOVE IN TIMER"))
	if(!UKismetMathLibrary::EqualEqual_VectorVector(Camera->GetComponentLocation(),MoveToComponent->GetSocketLocation(NameToMove),0.01f))
	{
		SetCameraProperties(UKismetMathLibrary::VLerp(Player->GetMesh()->GetSocketLocation(Camera->GetAttachSocketName()),
			MoveToComponent->GetSocketLocation(NameToMove),
			ADS_Curve.GetRichCurve()->Eval(ADSTimer)),
			UKismetMathLibrary::Lerp(90.0f,70.0f,FOV_Curve.GetRichCurve()->Eval(ADSTimer)));
		ADSTimer = (ADSTimer + 0.035f) < 1.0f ? ADSTimer + 0.035f : 1.0f;
	}
}

void UADSComponent::MoveOut()
{
	if(!Camera || !Player)
	{
		CurrentMoveStatus = None;
		return;
	}
	UE_LOG(LogADS,Display,TEXT("MOVE OUT TIMER"))
	if(!UKismetMathLibrary::EqualEqual_VectorVector(Camera->GetComponentLocation(),Player->GetMesh()->GetSocketLocation(Camera->GetAttachSocketName()),0.01f))
	{
		SetCameraProperties(UKismetMathLibrary::VLerp(Player->GetMesh()->GetSocketLocation(Camera->GetAttachSocketName()),
			Camera->GetComponentLocation(),
			ADS_Curve.GetRichCurve()->Eval(ADSTimer)),
			UKismetMathLibrary::Lerp(90.0f,70.0f,FOV_Curve.GetRichCurve()->Eval(ADSTimer)));
		ADSTimer = (ADSTimer - 0.045f) > 0.0f ? ADSTimer - 0.045f : 0.0f;
	}
	else
	{
		if(!GetWorld() || bADS) return;
		GetWorld()->GetTimerManager().ClearTimer(TimerADS);
		CurrentMoveStatus = None;
		ADSTimer = 0.0f;
		Camera->SetFieldOfView(90.0f);
		Camera->SetRelativeLocation(FVector::ZeroVector);
	}
}

void UADSComponent::ToggleADS(EMovementType MoveStatus)
{
	if(!GetWorld()) return;
	switch (MoveStatus) 
	{
	case EMovementType::MoveCameraIn:
		if(CurrentMoveStatus != MovingIn)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveIn,0.0075f,true);
			CurrentMoveStatus = MovingIn;
		}
		break;
	case EMovementType::MoveCameraOut:
		if(CurrentMoveStatus != MovingOut)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerADS,this, &UADSComponent::MoveOut,0.0075f,true);
			CurrentMoveStatus = MovingOut;
		}
		break;
	default:
		GetWorld()->GetTimerManager().ClearTimer(TimerADS);
		CurrentMoveStatus = None;
	}
}

// Debug Logic
FString UADSComponent::ADSInfo() const
{
	const FString DebugADS = "Is ADS: " + FString(bADS ? "True" : "False");
	const FString DebugComponent = "Move To Component: " + FString(MoveToComponent ? "Valid" : "Not Valid");
	const FString DebugName = "Socket Name: " + NameToMove.ToString();
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
	return (DebugADS + "\n" + DebugComponent + "\n" + DebugName + "\n" + DebugStatus + "\n");
}
