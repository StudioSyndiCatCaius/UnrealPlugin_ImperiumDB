// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Imperium_Settings.generated.h"


UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Imperium DB"))
class IMPERIUMDBRUNTIME_API UImperium_DevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Settings", meta=(MetaClass="Imperium_Project"))
	FSoftObjectPath DefaultSettings_Project;//{"/OmegaGameFramework/Settings/OmegaSettings_Gameplay_Demo_A.OmegaSettings_Gameplay_Demo_A"};
};
