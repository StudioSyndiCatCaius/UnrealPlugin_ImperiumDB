// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Imperium_Types.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Imperium_Subsystem.generated.h"

class AImperium_FlowInstance;
class AImperium_NodeInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImperiumFlowBegin, AImperium_FlowInstance*, Flow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImperiumFlowEnd, AImperium_FlowInstance*, Flow);

UCLASS()
class IMPERIUMDBRUNTIME_API UImperium_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY() TArray<AImperium_FlowInstance*> REF_FlowInstances;

public:
	void L_AddFlowInstance(AImperium_FlowInstance* inst);
	
	UPROPERTY(BlueprintAssignable) FOnImperiumFlowBegin OnImperiumFlowBegin;
	UPROPERTY(BlueprintAssignable) FOnImperiumFlowEnd OnImperiumFlowEnd;
	
	UFUNCTION(BlueprintCallable,Category="ImperiumDB|Subsystem")
	TArray<AImperium_FlowInstance*> GetAllActiveFlowInstances();

	UFUNCTION(BlueprintCallable,Category="ImperiumDB|Subsystem")
	AImperium_FlowInstance* StartFlowInstance(FImperium_Flow FlowData, TSubclassOf<AImperium_FlowInstance> InstanceClass,FImperium_FlowInstanceMeta meta);

	UFUNCTION(BlueprintCallable,Category="ImperiumDB|Subsystem")
	void StopAllActiveFlowInstances();
};
