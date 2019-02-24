// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Utils/Definitions.h"
#include "CharacterAnimator.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UCharacterAnimator : public UAnimInstance
{
	GENERATED_BODY()

public:

	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};
