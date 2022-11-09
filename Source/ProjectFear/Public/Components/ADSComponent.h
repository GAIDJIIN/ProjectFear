// Project Fear. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ADSComponent.generated.h"

// Forward Declaration
class UCameraComponent;

UENUM()
enum class EMovementType : uint8
{
	MoveCameraIn,
	MoveCameraOut
};

UENUM()
enum class EMoveStatus : uint8
{
	MovingIn,
	MovingOut,
	None
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFEAR_API UADSComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UADSComponent();
	
	// Functions
	UFUNCTION(BlueprintCallable,Category="ADS")
		void StartADS(USceneComponent* SceneComponentToMove, FName SocketName);
	UFUNCTION(BlueprintCallable,Category="ADS")
		void StopADS();
	UFUNCTION(BlueprintCallable,Category="ADS")
		void ToggleADS(EMovementType MoveStatus);
	
	// Getter
	UFUNCTION(BlueprintCallable,Category="Getter")
		bool IsADS() const {return bADS;}
	UFUNCTION(BlueprintCallable,Category="Getter")
		EMoveStatus GetMoveStatus() const { return CurrentMoveStatus; }
	// Debug
	UFUNCTION(BlueprintCallable)
	FString ADSInfo() const;
	
private:
	// Info
	// RealtimeCurves
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	FRuntimeFloatCurve ADS_Curve;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	FRuntimeFloatCurve FOV_Curve;
	
	// Info about ActorToMove
	USceneComponent* MoveToComponent = nullptr;
	FName NameToMove;
	
	// Timer
	FTimerHandle TimerADS;
    float ADSTimer = 0.0f;
	
	// Variables
	bool bADS = false;
	
	// Functions
	// For Start
	void StartToMoveCameraIn();
	void StartToMoveCameraOut();
	
	// For Timer
	void MoveIn();
	void MoveOut();
	
	// For Camera
	void SetCameraProperties(FVector CameraLocation, float CameraFOV) const;

	// Service
	void ClearMoveInfo();
	//void InitializeInfo();

	// Service Variables
	EMoveStatus CurrentMoveStatus = EMoveStatus::None;
};
