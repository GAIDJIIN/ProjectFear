// Project Fear. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/InteractInterface.h"
#include "CharacterCore.generated.h"

UCLASS(Abstract)
class PROJECTFEAR_API ACharacterCore : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterCore();
	// Setter
	UFUNCTION(BlueprintCallable, Category="Interact Logic|Set Info")
	void SetCanInteract(const bool bInteract);

	// Getter
	UFUNCTION(BlueprintCallable, Category="Interact Logic|Get Info")
	bool IsCanInteract() const { return InteractableActor != nullptr; }

	UFUNCTION(BlueprintCallable, Category="Interact Logic|Get Info")
	float GetDistanceToActor() const { return DistanceToActor; }

protected:
	virtual void BeginPlay() override;

	// Interact Info
	UPROPERTY(EditDefaultsOnly, Category="Interact Info|Start info")
	bool bCanInteract = true;
	UPROPERTY(EditDefaultsOnly, Category="Interact Info|Start info")
	float InteractTraceDistance = 175.0f;
	UPROPERTY(EditDefaultsOnly, Category="Interact Info|Start info")
	float TraceRadius = 2.5f;
	UPROPERTY(EditDefaultsOnly, Category="Interact Info|Debug info")
	bool bDrawInteractDebugTrace = false;
	// Interact Logic
	UFUNCTION(BlueprintCallable, Category="Interact Logic")
	void TryInteract();
	UFUNCTION(Category="Interact Logic")
	void StopInteract();
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	// Interact Info
	IInteractInterface* InteractableActor = nullptr;
	float DistanceToActor = InteractTraceDistance;
	// Interact Logic
	void FindInteract();
	void OnFindInteract(IInteractInterface* InteractActor);
	void OnLostInteract();
};
