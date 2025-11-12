// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Imperium_Types.h"
#include "JsonObjectWrapper.h"
#include "GameFramework/Actor.h"
#include "Imperium_Actors.generated.h"


class UImperium_Subsystem;
class UImperium_NodeSettings;
class AImperium_NodeInstance;
class UImperium_Project;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImpFlowDelegate, AImperium_FlowInstance*, Flow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImpNodeDelegate, AImperium_NodeInstance*, Node);

UCLASS()
class IMPERIUMDBRUNTIME_API AImperium_FlowInstance : public AActor
{
	GENERATED_BODY()

	UPROPERTY() UImperium_Subsystem* l_subsystem;
	UPROPERTY() UImperium_Project* l_ProjectSet=nullptr;
	UPROPERTY() UImperium_NodeSettings* l_NodeSet=nullptr;
	UPROPERTY() TArray<AImperium_NodeInstance*> REF_nodes;
	
	FImperium_AssetData L_GetFirstNodeOfTemplate();

public:
	UPROPERTY(BlueprintAssignable) FImpFlowDelegate OnImpFlowBegin;
	UPROPERTY(BlueprintAssignable) FImpFlowDelegate OnImpFlowEnd;
	UPROPERTY(BlueprintAssignable) FImpNodeDelegate OnImpFlow_NodeBegin;
	UPROPERTY(BlueprintAssignable) FImpNodeDelegate OnImpFlow_NodeEnd;
	
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION() void StartNode_ByRef(AImperium_NodeInstance* Node, int32 port);
	UFUNCTION() void StopNode_ByRef(AImperium_NodeInstance* Node);
	
	UPROPERTY(BlueprintReadOnly,Category="ImperiumDB") FImperium_Flow FlowData;
	UPROPERTY(BlueprintReadOnly,Category="ImperiumDB") FImperium_FlowInstanceMeta FlowMeta;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ImperiumDB")
	FJsonObjectWrapper JsonData;

	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	TArray<AImperium_NodeInstance*> GetAllActiveNodes();
	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	AImperium_NodeInstance* GetNode_FromLabel(const FString& Label);
	
	UFUNCTION(BlueprintCallable,Category="ImerpiumDB")
	bool StartNode_byLabel(FString NodeLabel,int32 NodePort);

	UFUNCTION(BlueprintCallable,Category="ImerpiumDB")
	void StopNode_ByLabel(FString NodeLabel);

	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	FImperium_FlowState GetFlowStateData();
	
	UFUNCTION(BlueprintCallable,Category="ImerpiumDB")
	void AttemptToEndFlow();
	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	void EndFlow();
};


UCLASS()
class IMPERIUMDBRUNTIME_API AImperium_NodeInstance : public AActor
{
	GENERATED_BODY()

	UPROPERTY() bool b_NodeActive;

public:
	
	// ---------------------------------------------------------
	// Delegates
	// ---------------------------------------------------------
	
	UPROPERTY(BlueprintAssignable) FImpNodeDelegate OnImpNodeBegin;
	UPROPERTY(BlueprintAssignable) FImpNodeDelegate OnImpNodeEnd;

	// ---------------------------------------------------------
	// Functions
	// ---------------------------------------------------------

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION() void Native_Input(int32 port);

	// ---------------------------------------------------------
	// Blueprint Params
	// ---------------------------------------------------------
	
	UPROPERTY() AImperium_FlowInstance* REF_owner;
	
	UPROPERTY(BlueprintReadOnly,Category="ImperiumDB")
	FImperium_AssetData NodeData;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ImperiumDB")
	FJsonObjectWrapper JsonData;

	// ---------------------------------------------------------
	// Blueprint Callabe
	// ---------------------------------------------------------
	
	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	FString GetNodeGlobalID() const;

	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB",meta=(AdvancedDisplay="ConnectedNode"))
	void TriggerOutput(int32 Port, bool bKillNode=true,int32 ConnectedNode=-1);

	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	bool IsNodeActive() const { return b_NodeActive; }

	UFUNCTION(BlueprintPure,Category="ImperiumDB")
	AImperium_FlowInstance* GetOwningInstance() const {return REF_owner;}
	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
	TArray<AImperium_NodeInstance*> GetConnections_ActiveNodes(bool bInput,bool bOutput) const;
	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB")
    TArray<FImperium_AssetData> GetConnections_NodesData(bool bInput,bool bOutput) const;

	//param
	UFUNCTION(BlueprintPure,Category="ImperiumDB") bool GetNodeParam_Bool(const FString& Param) const;
	UFUNCTION(BlueprintPure,Category="ImperiumDB") int32 GetNodeParam_Int(const FString& Param) const;
	UFUNCTION(BlueprintPure,Category="ImperiumDB") float GetNodeParam_Float(const FString& Param) const;
	UFUNCTION(BlueprintPure,Category="ImperiumDB") FString GetNodeParam_String(const FString& Param) const;
	
	// ---------------------------------------------------------
	// Blueprint Implementable
	// ---------------------------------------------------------
	
	UFUNCTION(BlueprintImplementableEvent,Category="ImperiumDB")
	void OnNodeBegin(int32 Port);

	UFUNCTION(BlueprintImplementableEvent,Category="ImperiumDB")
	void OnNodeEnd(int32 Port);

	UFUNCTION(BlueprintNativeEvent,Category="ImperiumDB")
	bool CanPlayNode();

};
