#include "SurfaceDefinition.h"
//==================================================================================================================================
//==================================================================================================================================
SBodySurface* SSurfaceDefinition::NormalSurface = 0;
//==================================================================================================================================
SBodySurface* SSurfaceDefinition::FrictionlessSurface = 0;
//==================================================================================================================================
SBodySurface* SSurfaceDefinition::StickySurface = 0;
//==================================================================================================================================
SBodySurface* SSurfaceDefinition::LadderSurface = 0;
//==================================================================================================================================
SBodySurface* SSurfaceDefinition::JumpThroughSurface = 0;
//==================================================================================================================================
void SSurfaceDefinition::Build()
{
	float scale = 60.f;

	NormalSurface = new SBodySurface();
	NormalSurface->fSurfaceAccXf = 12.0f * scale;
	NormalSurface->fSurfaceAccXb = 12.0f * scale;
	NormalSurface->fSurfaceDecX = 15.0f * scale;
	NormalSurface->fSurfaceMaxSpeedX = 5.0f * scale;
	NormalSurface->fSurfaceMaxSpeedY = 10.0f * scale;
	NormalSurface->fSurfaceAccY = 30.0f * scale;
	NormalSurface->fSurfaceDecY = 0.0f * scale;
	NormalSurface->fSurfaceSpeedUp = 0.0f;
	NormalSurface->fSurfaceSpeedDown = 0.0f;

	FrictionlessSurface = new SBodySurface();
	FrictionlessSurface->fSurfaceAccXf = 24.0f * scale;
	FrictionlessSurface->fSurfaceAccXb = 24.0f * scale;
	FrictionlessSurface->fSurfaceDecX = 0.0f * scale;
	FrictionlessSurface->fSurfaceMaxSpeedX = 12.0f * scale;
	FrictionlessSurface->fSurfaceMaxSpeedY = 30.0f * scale;
	FrictionlessSurface->fSurfaceAccY = 30.0f * scale;
	FrictionlessSurface->fSurfaceDecY = 0.0f * scale;
	FrictionlessSurface->fSurfaceSpeedUp = 0.0f;
	FrictionlessSurface->fSurfaceSpeedDown = 0.0f;

	StickySurface = new SBodySurface();
	StickySurface->fSurfaceAccXf = 1.0f * scale;
	StickySurface->fSurfaceAccXb = 15.0f * scale;
	StickySurface->fSurfaceDecX = 20.0f * scale;
	StickySurface->fSurfaceMaxSpeedX = 2.0f * scale;
	StickySurface->fSurfaceMaxSpeedY = 10.0f * scale;
	StickySurface->fSurfaceAccY = 30.0f * scale;
	StickySurface->fSurfaceDecY = 0.0f * scale;
	StickySurface->fSurfaceSpeedUp = 0.0f;
	StickySurface->fSurfaceSpeedDown = 0.0f;

	LadderSurface = new SBodySurface();
	LadderSurface->fSurfaceAccXf = 12.0f * scale;
	LadderSurface->fSurfaceAccXb = 12.0f * scale;
	LadderSurface->fSurfaceDecX = 60.0f * scale;
	LadderSurface->fSurfaceMaxSpeedX = 5.0f * scale;
	LadderSurface->fSurfaceMaxSpeedY = 10.0f * scale;
	LadderSurface->fSurfaceAccY = 0.0f * scale;
	LadderSurface->fSurfaceDecY = 1000.0f * scale;
	LadderSurface->fSurfaceSpeedUp = 120.0f * scale;
	LadderSurface->fSurfaceSpeedDown = 120.0f * scale;

	JumpThroughSurface = new SBodySurface();
	JumpThroughSurface->fSurfaceAccXf = 12.0f * scale;
	JumpThroughSurface->fSurfaceAccXb = 12.0f * scale;
	JumpThroughSurface->fSurfaceDecX = 15.0f * scale;
	JumpThroughSurface->fSurfaceMaxSpeedX = 5.0f * scale;
	JumpThroughSurface->fSurfaceMaxSpeedY = 10.0f * scale;
	JumpThroughSurface->fSurfaceAccY = 30.0f * scale;
	JumpThroughSurface->fSurfaceDecY = 0.0f * scale;
	JumpThroughSurface->fSurfaceSpeedUp = 0.0f * scale;
	JumpThroughSurface->fSurfaceSpeedDown = 0.0f * scale;
}
//==================================================================================================================================