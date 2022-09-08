// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ADS_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UADS_Interface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTFEAR_API IADS_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent)
	void GetMoveInfo(USceneComponent*& SceneComponent,FName& SocketToMove);
};
