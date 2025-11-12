// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Imperium_Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Imperium_Functions.generated.h"

class UImperium_NodeSettings;
class AImperium_NodeInstance;
class UImperium_Project;
UCLASS()
class IMPERIUMDBRUNTIME_API UImperium_Functions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	static UImperium_Project* GetImperiumProjectAsset();
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	static UImperium_NodeSettings* GetImperiumNodeSettings();
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	static FImperium_AssetData GotNodeData_FromLabel(FImperium_Flow flowData, const FString& label);

	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	static FImperium_Flow Conv_JsonToFlow(const FJsonObjectWrapper Json, const FString& Label);
	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	static TMap<FString, FString> GetFilesInPath(const FString& Path, const FString& Extension, bool bRecursive);

	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	static FString GetPathToFlow(const FString& FlowName);
};