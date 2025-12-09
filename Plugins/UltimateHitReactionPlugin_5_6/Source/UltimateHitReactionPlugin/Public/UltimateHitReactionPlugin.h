// Copyright StarExile Studio All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FUltimateHitReactionPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
