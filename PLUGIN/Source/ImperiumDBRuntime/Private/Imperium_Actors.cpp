// Fill out your copyright notice in the Description page of Project Settings.


#include "Imperium_Actors.h"

#include "Imperium_Assets.h"
#include "Imperium_Functions.h"
#include "Imperium_Subsystem.h"

// =====================================================================================================================
//  FLOW
// =====================================================================================================================

FImperium_AssetData AImperium_FlowInstance::L_GetFirstNodeOfTemplate()
{
	if(l_ProjectSet && l_ProjectSet->NodeSettings)
	{
		for(FImperium_AssetData a : FlowData.Nodes)
		{
			if(a.Template==l_ProjectSet->NodeSettings->NodeTemplate_DefaultStart)
			{
				return a;
			}
		}
	}
	return FImperium_AssetData();
}


void AImperium_FlowInstance::EndFlow()
{
	TArray<AImperium_NodeInstance*> _temp=GetAllActiveNodes();
	for(auto* a : _temp)
	{
		if(a)
		{
			a->K2_DestroyActor();
		}
	}
	if(l_subsystem)
	{
		l_subsystem->OnImperiumFlowEnd.Broadcast(this);
	}
	UE_LOG(LogTemp,Log,TEXT("Imperium - Ending Flow Instance: %s"),*GetName());
	OnImpFlowEnd.Broadcast(this);
	K2_DestroyActor();
}

void AImperium_FlowInstance::BeginPlay()
{
	UE_LOG(LogTemp,Log,TEXT("Begining Flow Instance: %s"),*GetName())
	if(UImperium_Project* _proj=UImperium_Functions::GetImperiumProjectAsset())
	{
		l_ProjectSet=_proj;
		if(UImperium_NodeSettings* _nod= UImperium_Functions::GetImperiumNodeSettings())
		{
			l_NodeSet=_nod;
		}
	}
	l_subsystem=GetGameInstance()->GetSubsystem<UImperium_Subsystem>();
	if(l_subsystem)
	{
		l_subsystem->L_AddFlowInstance(this);
		l_subsystem->OnImperiumFlowBegin.Broadcast(this);
	}
	bool _start_suceeded=false;

	//Override start
	if(FlowMeta.OverrideState.StartingNodes.Num()>0)
	{
		UE_LOG(LogTemp,Log,TEXT("Imperium - Starting FLOW %s as an OVERRIDE"),*FlowData.Label);
		_start_suceeded=true;
		for(FName n : FlowMeta.OverrideState.StartingNodes)
		{
			StartNode_byLabel(n.ToString(),0);
		}
	}
	// NORMAL START
	else
	{
		UE_LOG(LogTemp,Log,TEXT("Imperium - Starting FLOW %s as an NORMAL"),*FlowData.Label);
		//Get Starting node
		FString _firstNode=L_GetFirstNodeOfTemplate().Label;
		if(!FlowMeta.InputFlag.IsEmpty())
		{
			_firstNode=FlowMeta.InputFlag;
		}
		//Try and start first node
		StartNode_byLabel(_firstNode,0);
		_start_suceeded=true;
	}
	if(_start_suceeded)
	{
		OnImpFlowBegin.Broadcast(this);
	}
    Super::BeginPlay();
}

TArray<AImperium_NodeInstance*> AImperium_FlowInstance::GetAllActiveNodes()
{
	TArray<AImperium_NodeInstance*> out;
	for(auto* a : REF_nodes)
	{
		if(a && !a->IsActorBeingDestroyed() && a->IsNodeActive())
		{
			out.Add(a);
		}
	}
	return  out;
}

void AImperium_FlowInstance::StartNode_ByRef(AImperium_NodeInstance* Node, int32 port)
{
	if(Node && ! Node->IsNodeActive())
	{
		UE_LOG(LogTemp,Log,TEXT("Imperium - Starting node by reference: %s in port %i"),*Node->GetName(),port);
		REF_nodes.Add(Node);
		Node->Native_Input(port);
	}
}

void AImperium_FlowInstance::StopNode_ByRef(AImperium_NodeInstance* Node)
{
	if(Node)
	{
		if(REF_nodes.Contains(Node))
		{
			REF_nodes.Remove(Node);
			OnImpFlow_NodeEnd.Broadcast(Node);
			Node->K2_DestroyActor();
		}
	}
}

bool AImperium_FlowInstance::StartNode_byLabel(FString NodeLabel, int32 NodePort)
{
	UE_LOG(LogTemp,Log,TEXT("Imperium - Starting node with label: %s in port %i"),*NodeLabel,NodePort);
	if(l_NodeSet)
	{
		FImperium_AssetData _nodeData=UImperium_Functions::GotNodeData_FromLabel(FlowData,NodeLabel);
		TSubclassOf<AImperium_NodeInstance> _InClass=l_NodeSet->GetNodeClassFromTemplate(_nodeData.Template);
		AImperium_NodeInstance* _NewNode=GetWorld()->SpawnActorDeferred<AImperium_NodeInstance>(_InClass,FTransform());
		if(_NewNode)
		{
			_NewNode->NodeData=_nodeData;
			_NewNode->REF_owner=this;
			_NewNode->FinishSpawning(FTransform());
			OnImpFlow_NodeBegin.Broadcast(_NewNode);
			_NewNode->K2_AttachRootComponentToActor(this);
			StartNode_ByRef(_NewNode,NodePort);
		}
		return true;
	}
	return false;
}

void AImperium_FlowInstance::StopNode_ByLabel(FString NodeLabel)
{
	if(AImperium_NodeInstance* _node = GetNode_FromLabel(NodeLabel))
	{
		StopNode_ByRef(_node);
	}
}

FImperium_FlowState AImperium_FlowInstance::GetFlowStateData()
{
	FImperium_FlowState out;
	for(auto* n : GetAllActiveNodes())
	{
		out.StartingNodes.Add(*n->NodeData.Label);
	}
	return out;
}

void AImperium_FlowInstance::AttemptToEndFlow()
{
	if(GetAllActiveNodes().Num()<=0)
	{
		EndFlow();
	}
}


AImperium_NodeInstance* AImperium_FlowInstance::GetNode_FromLabel(const FString& Label)
{
	for(auto* n : GetAllActiveNodes())
	{
		if(n && n->NodeData.Label==Label)
		{
			return n;
		}
	}
	return nullptr;
}


// =====================================================================================================================
//  NODE
// =====================================================================================================================

void AImperium_NodeInstance::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(EndPlayReason==EEndPlayReason::Destroyed)
	{
		OnImpNodeEnd.Broadcast(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AImperium_NodeInstance::Native_Input(int32 port)
{
	b_NodeActive=true;
	OnImpNodeBegin.Broadcast(this);
	OnNodeBegin(port);
}

FString AImperium_NodeInstance::GetNodeGlobalID() const
{
	if(GetOwningInstance())
	{
		return GetOwningInstance()->FlowData.Label+"."+NodeData.Label;
	}
	return "NULL";
}

void AImperium_NodeInstance::TriggerOutput(int32 Port, bool bKillNode,int32 ConnectedNode)
{
	if(REF_owner && b_NodeActive)
	{
		b_NodeActive=false;
		int32 _checkedindex=0;
		for(FImperium_Connection c : REF_owner->FlowData.Connections)
		{
			if(c.from_node==NodeData.Label && c.from_port==Port)
			{
				if(ConnectedNode<0 || ConnectedNode==_checkedindex)
				{
					REF_owner->StartNode_byLabel(c.to_node,c.to_port);
				}
				_checkedindex+=1;
			}
		}
		if(bKillNode)
		{
			REF_owner->StopNode_ByRef(this);
			REF_owner->AttemptToEndFlow();			
		}
	}
}

TArray<AImperium_NodeInstance*> AImperium_NodeInstance::GetConnections_ActiveNodes(bool bInput, bool bOutput) const
{
	TArray<AImperium_NodeInstance*> out;
	if(REF_owner)
	{
		for(FImperium_Connection c : REF_owner->FlowData.Connections)
		{
			if(c.from_node==NodeData.Label && bOutput)
			{
				if(AImperium_NodeInstance* _nod=REF_owner->GetNode_FromLabel(c.to_node))
				{
					out.Add(_nod);
				}
			}
			if(c.to_node==NodeData.Label && bInput)
			{
				if(AImperium_NodeInstance* _nod=REF_owner->GetNode_FromLabel(c.from_node))
				{
					out.Add(_nod);
				}
			}
		}
	}
	return out;
}

TArray<FImperium_AssetData> AImperium_NodeInstance::GetConnections_NodesData(bool bInput, bool bOutput) const
{
	TArray<FImperium_AssetData> out;
	if(REF_owner)
	{
		for(FImperium_Connection c : REF_owner->FlowData.Connections)
		{
			if(c.from_node==NodeData.Label && bOutput)
			{
				out.Add(UImperium_Functions::GotNodeData_FromLabel(REF_owner->FlowData,c.to_node));
			}
			if(c.to_node==NodeData.Label && bInput)
			{
				out.Add(UImperium_Functions::GotNodeData_FromLabel(REF_owner->FlowData,c.from_node));
			}
		}
	}
	return out;
}

bool AImperium_NodeInstance::GetNodeParam_Bool(const FString& Param) const
{
	return NodeData.params.JsonObject->GetBoolField(Param);
}

int32 AImperium_NodeInstance::GetNodeParam_Int(const FString& Param) const
{
	return NodeData.params.JsonObject->GetIntegerField(Param);
}

float AImperium_NodeInstance::GetNodeParam_Float(const FString& Param) const
{
	return NodeData.params.JsonObject->GetNumberField(Param);
}

FString AImperium_NodeInstance::GetNodeParam_String(const FString& Param) const
{
	return NodeData.params.JsonObject->GetStringField(Param);
}

bool AImperium_NodeInstance::CanPlayNode_Implementation()
{
	return true;
}

