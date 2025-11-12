// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Engine/DataAsset.h"
#include "UObject/NoExportTypes.h"
#include "Imperium_Assets.generated.h"

class UImperium_NodeSettings;
class AImperium_FlowInstance;
class AImperium_NodeInstance;
/**
 * 
 */
UCLASS()
class IMPERIUMDBRUNTIME_API UImperium_Project : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ImperiumDB")
	FString LinkedProjectFile;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ImperiumDB")
	FJsonObjectWrapper JsonData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ImperiumDB")
	UImperium_NodeSettings* NodeSettings=nullptr;
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	FString GetDirectory_Root() const;
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	FString GetDirectory_Flow() const;
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	FString GetDirectory_Tables() const;
};

UCLASS()
class IMPERIUMDBRUNTIME_API UImperium_NodeSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION() TSubclassOf<AImperium_NodeInstance> GetNodeClassFromTemplate(const FString& Template);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Node")
	TSubclassOf<AImperium_FlowInstance> FlowClass_Default;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Node")
	FString NodeTemplate_DefaultStart;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Node")
	TSubclassOf<AImperium_NodeInstance> NodeClass_Default;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Node")
	TMap<FString,TSubclassOf<AImperium_NodeInstance>> NodeClass_ByTemplate;
};