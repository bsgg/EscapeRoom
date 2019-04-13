// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIBasicInteractiveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIBasicInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPEROOM_API IUIBasicInteractiveInterface
{
	GENERATED_BODY()
public:

	virtual void ExitUI() = 0;

	//virtual bool OnNavigate() = 0;

};
