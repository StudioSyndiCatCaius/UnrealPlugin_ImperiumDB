// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Imperium_Actors.h"
#include "Imperium_Subsystem.h"
#include "Imperium_Types.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Imperium_Tasks.generated.h"

class AImperium_FlowInstance;

UCLASS()
class IMPERIUMDBRUNTIME_API UAsyncAction_ImperiumFlow : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable) FOnImperiumFlowBegin Start;
	UPROPERTY(BlueprintAssignable) FOnImperiumFlowEnd Finish;

	UPROPERTY() const UObject* LocalWorldContext;
	UPROPERTY() FImperium_Flow Local_Data;
	UPROPERTY() AImperium_FlowInstance* Local_Instance;
	UPROPERTY() TSubclassOf<AImperium_FlowInstance> local_Class;
	UPROPERTY() FImperium_FlowInstanceMeta flow_meta;
	UFUNCTION() void Native_OnFinish(AImperium_FlowInstance* Flow);
	
	virtual void Activate() override;
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"), Category="ImperiumDB|AsyncGameplayTasks", meta = (WorldContext = "WorldContextObject")) 
	static UAsyncAction_ImperiumFlow* PlayImperiumFlowInstance(UObject* WorldContextObject, TSubclassOf<AImperium_FlowInstance> InstanceClass, FImperium_Flow FlowData, FImperium_FlowInstanceMeta meta);
	
};
