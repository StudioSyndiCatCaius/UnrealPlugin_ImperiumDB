// Fill out your copyright notice in the Description page of Project Settings.


#include "ImperiumDBFactory.h"

UImperiumDBFactory::UImperiumDBFactory()
{
	SupportedClass = UImperium_Project::StaticClass();
	bCreateNew = true;
}

UObject* UImperiumDBFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                               UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UImperium_Project>(InParent, Class, Name, Flags, Context);
}
