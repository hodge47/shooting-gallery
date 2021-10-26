// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TargetAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGALLERY_API UTargetAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	// Plays the target falling animation
	UFUNCTION(BlueprintCallable)
	void InitTargetFall();
	// Plays the target raising animation
	UFUNCTION(BlueprintCallable)
	void InitTargetRaise();


private:
	// The reference to the target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation", meta = (AllowPrivateAccess = "true"))
	class ATarget* Target;
	// The active state of the target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsTargetActive;
	// The state of the target falling
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsTargetFalling;
	// The state of the target raising
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsTargetRaising;
	
};
