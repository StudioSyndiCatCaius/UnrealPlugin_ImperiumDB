// Fill out your copyright notice in the Description page of Project Settings.


#include "Imperium_Tasks.h"

#include "Imperium_Subsystem.h"

void UAsyncAction_ImperiumFlow::Native_OnFinish(AImperium_FlowInstance* Flow)
{
	Finish.Broadcast(Local_Instance);
	SetReadyToDestroy();
}

void UAsyncAction_ImperiumFlow::Activate()
{
	if(LocalWorldContext)
	{
		UImperium_Subsystem* _subsystem=LocalWorldContext->GetWorld()->GetGameInstance()->GetSubsystem<UImperium_Subsystem>();
		_subsystem->OnImperiumFlowEnd.AddDynamic(this, &UAsyncAction_ImperiumFlow::Native_OnFinish);
		if(AImperium_FlowInstance* _inst=_subsystem->StartFlowInstance(Local_Data,local_Class,flow_meta))
		{
			Start.Broadcast(_inst);
			Local_Instance=_inst;
		}
		else
		{
			SetReadyToDestroy();
		}
	}
	Super::Activate();
}

UAsyncAction_ImperiumFlow* UAsyncAction_ImperiumFlow::PlayImperiumFlowInstance(UObject* WorldContextObject, TSubclassOf<AImperium_FlowInstance> InstanceClass, 
	FImperium_Flow FlowData, FImperium_FlowInstanceMeta meta)
{
	UAsyncAction_ImperiumFlow* NewNode = NewObject<UAsyncAction_ImperiumFlow>();
	NewNode->LocalWorldContext=WorldContextObject;
	NewNode->Local_Data=FlowData;
	NewNode->flow_meta=meta;
	if(InstanceClass)
	{
		NewNode->local_Class=InstanceClass;
	}
	return NewNode;
}

