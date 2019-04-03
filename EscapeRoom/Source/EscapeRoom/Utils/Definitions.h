// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Definitions.generated.h"

const static FName NONE_TAG = TEXT("None");

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
	VE_INSPECT		    UMETA(DisplayName = "Inspect"),
	VE_NONE 		    UMETA(DisplayName = "None"),
};

UENUM(BlueprintType) // Type interaction
enum class EDirectionType : uint8
{
	VE_NONE 		UMETA(DisplayName = "None"),
	VE_UP		    UMETA(DisplayName = "Up"),
	VE_DOWN		    UMETA(DisplayName = "Down"),
	VE_RIGHT		UMETA(DisplayName = "Right"),
	VE_LEFT		    UMETA(DisplayName = "Left"),
	
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
		Name = "Object Name";
		ID = "Obj_";
		//ObjectType = EObjectType::VE_COMPLETE;
		Thumbnail = nullptr;
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> ObjectMesh;



	// Override equal operator for this structure
	bool operator==(const FObjectInteraction& object) const
	{
		return (ID == object.ID);
	}
};

USTRUCT(BlueprintType)
struct FObjectCombination : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID_Result;
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



USTRUCT(BlueprintType)
struct FActionDefinition
{
	GENERATED_BODY()

public:

	FActionDefinition()
	{
		ObjectID = "NONE";
		AnimationLength = 0.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DetailDefaultAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DetailWrongAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DestroyWhenActionCompleted = false;

	bool HasObject()
	{
		return (ObjectID.ToString().ToLower() != "none");
	}

	bool HasAnimation()
	{
		return (AnimationLength > 0.0f);
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
