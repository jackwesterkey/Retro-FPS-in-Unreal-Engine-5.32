// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemInterface.h"
#include "Shooty.generated.h"

UCLASS()
class MYPROJECT_API AShooty : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AShooty();

    // Interface implementation
    UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Visible anywhere but only readable, used for abilities
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
    class UAbilitySystemComponent* AbilitySystemComponent;

    // Attributes for this character
    UPROPERTY()
    class URetroAttributeSet* Attributes;

public:
    // Default effect for attributes
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RetroFPS")
    TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

    // Default abilities for this character
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RetroFPS")
    TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;
};
