// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "ADSComponent.generated.h"


UENUM()
enum class EMovementType : uint8
{
	MoveCameraIn,
	MoveCameraOut
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFEAR_API UADSComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UADSComponent();
	
	// Functions
	void StartADS(AActor* ActorToMove, EMovementType MoveStatus);
protected:
	virtual void BeginPlay() override;

private:
	// Info for Move Camera
	AActor* ActorToMove = nullptr;
	UCameraComponent* Camera = nullptr;

	// Timer
	FTimerHandle TimerADS;
    float ADSTimer = 0.0f;
	
	// Variables
	bool bADS = false;
	ACharacter* Player = nullptr;

	// RealtimeCurves
	FRuntimeFloatCurve ADS_Curve;
	FRuntimeFloatCurve FOV_Curve;
	// Functions
	// For Start
	void StartToMoveCameraIn();
	void StartToMoveCameraOut();
	// For Timer
	void MoveIn();
	void MoveOut();
};
