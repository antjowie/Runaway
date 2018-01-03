//
// This exists because launcher is programmed so badly
//

#pragma once
#include "Launcher.h"
class BossLauncher :
	public Launcher
{
public:
	void update(const float elapsedTime) override final;
	// Used to predict player movement
	float getProjectileSpeed() const;
	BossLauncher();
};

