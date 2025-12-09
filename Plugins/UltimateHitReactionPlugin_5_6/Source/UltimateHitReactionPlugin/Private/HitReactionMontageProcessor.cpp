// Copyright StarExile Studio All Rights Reserved.


#include "HitReactionMontageProcessor.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h" // <-- 添加头文件以防UAnimSequence未包含
#include "EditorUtilityLibrary.h"
#include "Chooser.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "InstancedStruct.h"
#include "StructUtils/StructUtils.h"
#include "ObjectChooser_Asset.h"
#include "Logging/LogMacros.h" // <-- 确保包含日志头文件

DEFINE_LOG_CATEGORY_STATIC(LogHitReactionProcessor, Log, All); // 定义本地日志类别

UHitReactionMontageProcessor::UHitReactionMontageProcessor()
{
	SupportedClasses.Add(UAnimMontage::StaticClass());
	SupportedClasses.Add(UChooserTable::StaticClass());
}

void UHitReactionMontageProcessor::ProcessMontage_SlotName()
{
	TArray<UObject*> MontageAssets = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UAnimMontage::StaticClass());
	if (MontageAssets.IsEmpty())
	{
		UE_LOG(LogHitReactionProcessor, Warning, TEXT("No Montage assets selected."));
		return;
	}

	if (!Cast<UAnimMontage>(MontageAssets[0]))
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("First selected asset is not a valid UAnimMontage."));
		return;
	}

	USkeleton* TargetSkeleton = Cast<UAnimMontage>(MontageAssets[0])->GetSkeleton();

	if (!TargetSkeleton)
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("Failed to get skeleton from the first montage asset."));
		return;
	}

	FAnimSlotGroup* DefaultHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("DefaultGroup");
	FAnimSlotGroup* AdditiveHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("AdditiveGroup");
	if (!DefaultHitReactionSlotGroup)
	{
		TargetSkeleton->AddSlotGroupName("DefaultGroup");
		DefaultHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("DefaultGroup");
	}
	if (!AdditiveHitReactionSlotGroup)
	{
		TargetSkeleton->AddSlotGroupName("DefaultGroup");
		AdditiveHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("AdditiveGroup");
	}
	if (DefaultHitReactionSlotGroup && AdditiveHitReactionSlotGroup)
	{
		TargetSkeleton->SetSlotGroupName("HitReaction_InPlace", "AdditiveGroup");
		TargetSkeleton->SetSlotGroupName("HitReaction_RootMotion", "DefaultGroup");
	}
	else
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("Failed to create or find required slot groups."));
		return;
	};

	for (UObject* Montage : MontageAssets)
	{
		if (!Montage->GetName().Contains("HitReaction"))
			continue;
		if (!Cast<UAnimMontage>(Montage)->SlotAnimTracks.IsEmpty() &&
			!Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments.IsEmpty() &&
			Cast<UAnimMontage>(Montage)->GetSkeleton() == TargetSkeleton)
		{
			// Light
			if (Montage->GetName().Contains("Light"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 0.35f;
				
				if (!Montage->GetName().Contains("Death"))
				{
					Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_InPlace";
				}
				else
				{
					Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				}
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Medium
			else if (Montage->GetName().Contains("Medium"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 0.7f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Heavy
			else if(Montage->GetName().Contains("Heavy"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 1.15f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Explosion & No Death
			else if (Montage->GetName().Contains("Explosion"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 1.0f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}

		}
	}
}


void UHitReactionMontageProcessor::ProcessMontage_SequenceSlot()
{
	TArray<UObject*> MontageAssets = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UAnimMontage::StaticClass());
	TArray<UObject*> HitReactionSequences = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UAnimSequence::StaticClass());

	if (HitReactionSequences.IsEmpty() || MontageAssets.IsEmpty())
	{
		UE_LOG(LogHitReactionProcessor, Warning, TEXT("Either Montage assets or HitReaction sequences are not selected."));
		return;
	}

	if (!Cast<UAnimSequence>(HitReactionSequences[0]))
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("First selected asset is not a valid UAnimSequence."));
		return;
	}

	USkeleton* TargetSkeleton = Cast<UAnimSequence>(HitReactionSequences[0])->GetSkeleton();

	if (!TargetSkeleton)
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("Failed to get skeleton from first animation sequence."));
		return;
	}


	FAnimSlotGroup* DefaultHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("DefaultGroup");
	FAnimSlotGroup* AdditiveHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("AdditiveGroup");
	if (!DefaultHitReactionSlotGroup)
	{
		TargetSkeleton->AddSlotGroupName("DefaultGroup");
		DefaultHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("DefaultGroup");
	}
	if (!AdditiveHitReactionSlotGroup)
	{
		TargetSkeleton->AddSlotGroupName("AdditiveGroup");
		AdditiveHitReactionSlotGroup = TargetSkeleton->FindAnimSlotGroup("AdditiveGroup");
	}
	if (DefaultHitReactionSlotGroup && AdditiveHitReactionSlotGroup)
	{
		TargetSkeleton->SetSlotGroupName("HitReaction_InPlace", "AdditiveGroup");
		TargetSkeleton->SetSlotGroupName("HitReaction_RootMotion", "DefaultGroup");
	}
	else
	{
		UE_LOG(LogHitReactionProcessor, Error, TEXT("Failed to create or find required slot groups."));
		return;
	}

	for (UObject* Montage : MontageAssets)
	{
		if (!Montage->GetName().Contains("HitReaction"))
			continue;
		for (UObject* Sequence : HitReactionSequences)
		{
			if (!Montage->GetName().Contains("HitReaction"))
				continue;
			FString left, SequenceName;
			bool IsSequence = Sequence->GetName().Split("AS_", &left, &SequenceName);

			if (!IsSequence)
				continue;

			if (Montage->GetName().Contains(Sequence->GetName())||
				Montage->GetName().Contains(SequenceName))
			{
				if (!Cast<UAnimMontage>(Montage)->SlotAnimTracks.IsEmpty() && 
					!Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments.IsEmpty())
				{
					Cast<UAnimMontage>(Montage)->SetSkeleton(Cast<UAnimSequence>(Sequence)->GetSkeleton());
					Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].SetAnimReference(Cast<UAnimSequence>(Sequence));
					Cast<UAnimMontage>(Montage)->MarkPackageDirty();
					break;
				}	
				
			}
		}

		// Set Anim Sequence Time Range to match the best behavior
		if (!Cast<UAnimMontage>(Montage)->SlotAnimTracks.IsEmpty() &&
			!Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments.IsEmpty())
		{
			// Light & No Death
			if (Montage->GetName().Contains("Light"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 0.2f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				if (!Montage->GetName().Contains("Death"))
				{
					Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_InPlace";
				}
				else
				{
					Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				}
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Medium
			else if (Montage->GetName().Contains("Medium"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 0.7f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Heavy
			else if (Montage->GetName().Contains("Heavy"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 1.15f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
			// Explosion & No Death
			else if (Montage->GetName().Contains("Explosion"))
			{
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimStartTime = 1.0f;
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].UpdateCachedPlayLength();
				Cast<UAnimMontage>(Montage)->SetCompositeLength(Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].AnimTrack.AnimSegments[0].CachedPlayLength);
				Cast<UAnimMontage>(Montage)->SlotAnimTracks[0].SlotName = "HitReaction_RootMotion";
				Cast<UAnimMontage>(Montage)->GetSkeleton()->MarkPackageDirty();
				Cast<UAnimMontage>(Montage)->MarkPackageDirty();
			}
		}
	}
}
void UHitReactionMontageProcessor::ProcessMontage_Chooser(FName TargetMontagesPath)
{
	TArray<UObject*> Choosers = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UChooserTable::StaticClass());
	TArray<FAssetData> MontageData;
	IAssetRegistry::Get()->GetAssetsByPath(TargetMontagesPath, MontageData);

	if (Choosers.IsEmpty() || MontageData.IsEmpty())
	{
		UE_LOG(LogHitReactionProcessor, Warning, TEXT("No Choosers selected or no Montage data found at given path."));
		return;
	}

	UChooserTable* TargetChooserTable = Cast<UChooserTable>(Choosers[0]);

	for (TObjectPtr<UChooserTable> Chooser : TargetChooserTable->NestedChoosers)
	{
		for (FInstancedStruct& Instance : Chooser->ResultsStructs)
		{
			if(Instance.GetScriptStruct()->GetName() != "NestedChooser" 
				&& Instance.GetScriptStruct()->GetName() == "SoftAssetChooser")
			{
				FSoftAssetChooser& AssetChooser = Instance.GetMutable<FSoftAssetChooser>();
				for (FAssetData& MontageAssetData : MontageData)
				{
					if (!MontageAssetData.IsValid())
						continue;
					UObject* MontageAsset = MontageAssetData.GetAsset();
					if(MontageAsset && MontageAsset->GetName() == AssetChooser.Asset.LoadSynchronous()->GetName())
					{
						AssetChooser.Asset = MontageAsset;
					}
				}
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *AssetChooser.Asset->GetName());
			}

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Chooser process finished"));
	TargetChooserTable->MarkPackageDirty();
}
