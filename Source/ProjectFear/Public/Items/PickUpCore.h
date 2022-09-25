// Project Fear. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/InteractInterface.h"
#include "GameFramework/Character.h"
#include "PickUpCore.generated.h"


UCLASS(Abstract)
class PROJECTFEAR_API APickUpCore : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	APickUpCore();
protected:
	virtual void BeginPlay() override;
	// Interact Logic
	virtual void StartInteract(const ACharacter* Player) override;
	virtual void StopInteract() override;
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void StartInteractLogic(const ACharacter* Player);
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void StopInteractLogic();

	// Interact Hovered Logic
	virtual void StartHover() override;
	virtual void StopHover() override;
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void StartHoverLogic();
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void StopHoverLogic();
};
