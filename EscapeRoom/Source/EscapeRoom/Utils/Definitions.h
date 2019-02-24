// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Definitions.generated.h"

UENUM(BlueprintType) // Player Type
enum class ECharacterType : uint8
{
	VE_RACOON		UMETA(DisplayName = "Racoon"),
	VE_CAT			UMETA(DisplayName = "Cat"),
	VE_NONE			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType) // Type interaction
enum class EObjectType : uint8
{
	VE_COMPLETE		UMETA(DisplayName = "Complete Object"),
	VE_PART			UMETA(DisplayName = "Part Object"), // Item part of other items
	
};

UENUM(BlueprintType) // Type interaction
enum class EGestureType : uint8
{
	VE_INTERACT		    UMETA(DisplayName = "Interact"),
	VE_DISMISS		    UMETA(DisplayName = "Dismiss"),
	VE_NONE 		    UMETA(DisplayName = "None"),
};

UENUM(BlueprintType) // Type interaction
enum class EInteractionType : uint8
{
	VE_NONE 		UMETA(DisplayName = "None"),
	VE_VIEW		    UMETA(DisplayName = "View"),
	VE_USE		    UMETA(DisplayName = "Use"),
	VE_PICKUP		UMETA(DisplayName = "Pick up"),
};

UENUM(BlueprintType) // Type interaction
enum class EInteractionStatus : uint8
{
	VE_NONE 		UMETA(DisplayName = "None"),
	VE_LOCKED		    UMETA(DisplayName = "Locked"),
	VE_UNLOCKED		    UMETA(DisplayName = "Unlocked"),
	VE_ACTIVE		UMETA(DisplayName = "Active"),
	VE_INACTIVE		UMETA(DisplayName = "Inactive"),
};



template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
};

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->FindEnumIndex(FName(*String));
};

USTRUCT(BlueprintType)
struct FCharacterType
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType = ECharacterType::VE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAvailable = false;
};




USTRUCT(BlueprintType)
struct FObjectInteraction : public FTableRowBase
{
	GENERATED_BODY()

public:

	FObjectInteraction()
	{
		Name = FText::FromString("Object Name");
		ID = "Obj_";
		ObjectType = EObjectType::VE_COMPLETE;
		Thumbnail = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType ObjectType;

	// Parent ID to form a complete object (only possible for PART OBJECTS)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ID to form an object"))
	FName ParentID;

	// List of object ID Parts to form this item (only possible for COMPLETE OBJECTS)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "List of ID Parts to complete this object"))
	TArray<FName> IDObjectParts;

	// Override equal operator for this structure
	bool operator==(const FObjectInteraction& object) const
	{
		return (ID == object.ID);
	}
};

USTRUCT(BlueprintType)
struct FActionData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FActionData()
	{
		DescriptionIndex = 0;
		InteractionType = EInteractionType::VE_NONE;
		Active = true;
	}


	// Object required or object to take
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID;

	// Interaction type
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionType InteractionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName InteractionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Active;

	// Depending on Action type:
	// View: Description goes in loop from 0 to num actions
	// Pickup:
	// Use: ID 0 = Use correct (you have the required object)
	//	    ID 1 = Use incorrect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> DescriptionList;

	// Index for other uses. For example in some cases we want to show 
	// the description (VIEW Action) until this index and after that enabled the rest of the actions
	int SpecialDescriptionIndex;

	FName GetDescriptionById(int index) const
	{
		if ((DescriptionList.Num() > 0) && (index < DescriptionList.Num()))
		{
			return DescriptionList[index];
		}

		return "EMPTY";
	}

	FName GetSuccessDescription() const
	{
		if (DescriptionList.Num() > 0)
		{
			return DescriptionList[0];
		}

		int rand = FMath::RandRange(0, 2);

		if (rand == 0)
		{
			return "Let's do this";

		}

		if (rand == 1)
		{
			return "That's it!";
		}


		if (rand == 2)
		{
			return "Now we are talking";
		}

		return "";
	}

	FName GetWrongDescription() const
	{
		if (DescriptionList.Num() > 1)
		{
			return DescriptionList[1];
		}

		int rand = FMath::RandRange(0, 2);

		if (rand == 0)
		{
			return "That doesn't make any sense";

		}

		if (rand == 1)
		{
			return "I don't understand what you want";
		}


		if (rand == 2)
		{
			return "Let's think this through, sall we?";
		}

		return "";
	}

	FName GetDescription() const
	{
		if ((DescriptionList.Num() > 0) && (DescriptionIndex < DescriptionList.Num()))
		{
			return DescriptionList[DescriptionIndex];
		}

		return "";
	}

	int GetDescriptionIndex() const
	{
		return DescriptionIndex;
	}

	void SetDescriptionIndex(int newIndex)
	{
		
		DescriptionIndex = newIndex;
	}

	void UpdateIndex(int newIndex)
	{
		Index = newIndex;

		if (Index < 0)
		{
			Index = 0;
		}

		if (Index >= DescriptionList.Num())
		{
			Index = DescriptionList.Num() - 1;
		}
	}

	// Override equal operator for this structure
	bool operator==(const FActionData& Action) const
	{
		return (InteractionType == Action.InteractionType);
	}

private:

	// Descriptions
	int DescriptionIndex;
};


USTRUCT(BlueprintType)
struct FInteractionData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FInteractionData()
	{
		Name = FText::FromString("Interactable");
	}

	// Name interactable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionStatus Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActionData PrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActionData SecondaryAction;

	// Override equal operator for this structure
	bool operator==(const FInteractionData& object) const
	{
		return (Name.ToString() == object.Name.ToString());
	}

};




USTRUCT(BlueprintType)
struct FGestureAnimation
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGestureType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationTime;

	FGestureAnimation()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		Type = EGestureType::VE_NONE;
		AnimationTime = 0.0f;
	}
};


/**
 * 
 */
class ESCAPEROOM_API Definitions
{
public:
	Definitions();
	~Definitions();
};
