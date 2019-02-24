
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"
#include "Utils/Definitions.h"

#include "EscapeRoomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AEscapeRoomPlayerState : public APlayerState
{
	GENERATED_BODY()



public:

	UPROPERTY(Replicated)
	ECharacterType SelectedCharacter = ECharacterType::VE_NONE;

	//FORCEINLINE ECharacterType GetSelectedCharacter() const { return CurrentSelectedCharacter; }

	//FORCEINLINE void SetSelectedCharacter(ECharacterType NewCharacterType) { CurrentSelectedCharacter = NewCharacterType; }
	
};
