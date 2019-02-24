// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoomPlayerState.h"
#include "UnrealNetwork.h"


void AEscapeRoomPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	 
	DOREPLIFETIME(AEscapeRoomPlayerState, SelectedCharacter); 
	   
}

