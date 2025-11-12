// Fill out your copyright notice in the Description page of Project Settings.


#include "Imperium_Subsystem.h"

#include "Imperium_Actors.h"
#include "Imperium_Assets.h"
#include "Imperium_Functions.h"

void UImperium_Subsystem::L_AddFlowInstance(AImperium_FlowInstance* inst)
{
	if(inst && !REF_FlowInstances.Contains(inst))
	{
		REF_FlowInstances.Add(inst);
	}
}

TArray<AImperium_FlowInstance*> UImperium_Subsystem::GetAllActiveFlowInstances()
{
	TArray<AImperium_FlowInstance*> out;
	for(auto* a : REF_FlowInstances)
	{
		if(a && !a->IsActorBeingDestroyed())
		{
			out.Add(a);
		}
	}
	REF_FlowInstances=out;
	return out;
}

AImperium_FlowInstance* UImperium_Subsystem::StartFlowInstance(FImperium_Flow FlowData,TSubclassOf<AImperium_FlowInstance> InstanceClass,
	FImperium_FlowInstanceMeta meta)
{
	TSubclassOf<AImperium_FlowInstance> def_class=AImperium_FlowInstance::StaticClass();
	if(InstanceClass) {def_class=InstanceClass;}
	if(UImperium_NodeSettings* _set= UImperium_Functions::GetImperiumNodeSettings())
	{
		if(_set->FlowClass_Default) {def_class=_set->FlowClass_Default;}
	}
	AImperium_FlowInstance* _newInst=GetWorld()->SpawnActorDeferred<AImperium_FlowInstance>(def_class,FTransform());
	if(_newInst)
	{
		_newInst->FlowData=FlowData;
		_newInst->FlowMeta=meta;
		_newInst->FinishSpawning(FTransform());
		return _newInst;
	}
	return nullptr;
}

void UImperium_Subsystem::StopAllActiveFlowInstances()
{
	for(auto* a : GetAllActiveFlowInstances())
	{
		if(a)
		{
			a->AttemptToEndFlow();
		}
	}
}
