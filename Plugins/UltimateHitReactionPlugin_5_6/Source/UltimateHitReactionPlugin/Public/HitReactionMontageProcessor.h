// Copyright StarExile Studio All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "HitReactionMontageProcessor.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATEHITREACTIONPLUGIN_API UHitReactionMontageProcessor : public UAssetActionUtility
{
	GENERATED_BODY()
public:
	UHitReactionMontageProcessor();

	UFUNCTION(BlueprintCallable, meta = (Category = "Ultimate Hit Reaction", CallInEditor = true, ToolTip = "This plugin is implemented based on string matching. Please do not use this plugin to process any animations outside of the HitReaction animation pack"))
	void ProcessMontage_SequenceSlot();

	UFUNCTION(BlueprintCallable, meta = (Category = "Ultimate Hit Reaction", CallInEditor = true, ToolTip = "This plugin is implemented based on string matching. Please do not use this plugin to process any animations outside of the HitReaction animation pack"))
	void ProcessMontage_SlotName();
	UFUNCTION(BlueprintCallable, meta = (Category = "Ultimate Hit Reaction", CallInEditor = true, ToolTip = "This plugin is implemented based on string matching. Please do not use this plugin to process any animations outside of the HitReaction animation pack"))
	void ProcessMontage_Chooser(FName TargetMontagesPath = "/Game/");
};
