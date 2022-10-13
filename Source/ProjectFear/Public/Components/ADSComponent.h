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

	// Debug
	UFUNCTION(BlueprintCallable)
	FString ADSInfo() const;

	// Info
	// RealtimeCurves
	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve ADS_Curve;
	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve FOV_Curve;
protected:
	virtual void BeginPlay() override;
	
private:
	// Info for Move Camera
	UCameraComponent* Camera = nullptr;
	
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
	void InitializeInfo();

	// Service Variables
	enum EMoveStatus {MovingIn,MovingOut,None} CurrentMoveStatus = None;
};
