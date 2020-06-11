// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsEventHandler.generated.h"


typedef TBaseDelegate<void, UObject*> FEventDelegate; // Define my basic delegate -_-'

// this is to manage all my delegates
static TMap<FString, TArray<FEventDelegate*>> EventManager;

/**
 * 
 */
UCLASS()
class TSEVENTMANAGER_API UTsEventHandler : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

	UFUNCTION(BlueprintCallable, Category = "Event Manager")
	static void RegisterAction(FString& EventName, const FString& _functionName, UObject* InObject)
	{
		if (!_functionName.IsEmpty() && !EventName.IsEmpty())
		{
			FEventDelegate t_del;

			t_del.BindUFunction(InObject, FName(*_functionName)); // Function name should be MyClass::HelloWorld()

			
			if (EventManager.Find(EventName))
			{
				EventManager[EventName].Add(&t_del);
			}
			else
			{
				TArray<FEventDelegate*> DelegateStorage = {&t_del};

				EventManager.Add(EventName, DelegateStorage);
			}
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Event Manager")
	static void NofityAction(FString& EventName, UObject* Params)
	{
		if (!EventManager.Find(EventName))
			return;

		// Go through Event Manager 
		for (int i = 0; i < EventManager[EventName].Num(); i++)
		{
			EventManager[EventName][i]->ExecuteIfBound(Params);
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Event Manager")
	static void UnRegisterAction(FString EventName)
	{
		if (!EventManager.Find(EventName))
			return;

		// Go through Event Manager 
		for (int i = 0; i < EventManager[EventName].Num(); i++)
		{
			EventManager[EventName][i]->Unbind();
		}

		EventManager.Remove(EventName);
	}

};
