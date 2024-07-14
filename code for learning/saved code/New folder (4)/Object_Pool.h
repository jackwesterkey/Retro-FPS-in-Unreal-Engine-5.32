// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h"
#include "Object_Pool.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UObject_Pool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObject_Pool();

	UFUNCTION(Blueprintcallable, Category = "Object_Pool")
	APooledObject* SpawnPooledObject();

	UPROPERTY(EditAnywhere, Category = "Object_Pool")
	TSubclassOf<class APooledObject> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = "Object_Pool")
	int PoolSize = 20;

	UPROPERTY(EditAnywhere, Category = "Object_Pool")
	float PooledObjectLifeSpan = 0.0f;

	UFUNCTION()
	void OnPooledObjectDespawn(APooledObject* PoolActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<APooledObject*> ObjectPool;
	TArray<int> SpawnedPoolIndexes;
};
