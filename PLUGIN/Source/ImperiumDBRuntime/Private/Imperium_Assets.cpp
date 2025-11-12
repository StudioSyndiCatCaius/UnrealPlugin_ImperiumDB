// Fill out your copyright notice in the Description page of Project Settings.


#include "Imperium_Assets.h"

#include "Kismet/KismetStringLibrary.h"

FString UImperium_Project::GetDirectory_Root() const
{
	FString L;
	FString R;
	LinkedProjectFile.Split("/",&L,&R,ESearchCase::IgnoreCase,ESearchDir::FromEnd);

	L=UKismetStringLibrary::Replace(L,"{project}",FPaths::ProjectDir());
	L=UKismetStringLibrary::Replace(L,"{content}",FPaths::ProjectContentDir());
	
	return L;
}

FString UImperium_Project::GetDirectory_Flow() const
{
	return GetDirectory_Root()+"/flow/";
}

FString UImperium_Project::GetDirectory_Tables() const
{
	
	return GetDirectory_Root()+"/tables/";
}

TSubclassOf<AImperium_NodeInstance> UImperium_NodeSettings::GetNodeClassFromTemplate(const FString& Template)
{
	if(NodeClass_ByTemplate.Contains(Template))
	{
		return NodeClass_ByTemplate.FindOrAdd(Template);
	}
	return NodeClass_Default;
}
