// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"

#include "Imperium_Types.generated.h"


USTRUCT(Blueprintable,BlueprintType)
struct FImperium_Connection
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString from_node;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	int32 from_port;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString to_node;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	int32 to_port;
};

USTRUCT(Blueprintable,BlueprintType)
struct FImperium_AssetData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString Template;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString Label;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FName key;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FJsonObjectWrapper meta;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FJsonObjectWrapper params;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	TArray<FName> tags;
};

USTRUCT(Blueprintable,BlueprintType)
struct FImperium_Flow
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString Label;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	TArray<FImperium_Connection> Connections;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	TArray<FImperium_AssetData> Nodes;
};


USTRUCT(Blueprintable,BlueprintType)
struct FImperium_FlowState
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	TArray<FName> StartingNodes;
};


USTRUCT(Blueprintable,BlueprintType)
struct FImperium_FlowInstanceMeta
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FString InputFlag;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Imperium")
	FImperium_FlowState OverrideState;
};

