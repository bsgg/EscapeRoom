// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimator.h"
#include "UnrealNetwork.h"

//This function is like PostInitAnimtree in UE3
void UCharacterAnimator::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	
}

//Tick
void UCharacterAnimator::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn)
	{
		return;
	}
}
