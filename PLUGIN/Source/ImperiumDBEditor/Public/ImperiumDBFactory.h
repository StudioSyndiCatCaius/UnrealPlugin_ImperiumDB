// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Imperium_Assets.h"
#include "Factories/Factory.h"
#include "ImperiumDBFactory.generated.h"


class FImperiumDBAssetTypeActions : public FAssetTypeActions_Base
{
public:
	UClass* GetSupportedClass() const override {return UImperium_Project::StaticClass();};
	FText GetName() const override { return INVTEXT("Actor Preset"); };
	FColor GetTypeColor() const override {return FColor::Cyan;};
	uint32 GetCategories() override { return EAssetTypeCategories::Misc; };
};


UCLASS()
class IMPERIUMDBEDITOR_API UImperiumDBFactory : public UFactory
{
	GENERATED_BODY()
public:
	UImperiumDBFactory();
	UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};

