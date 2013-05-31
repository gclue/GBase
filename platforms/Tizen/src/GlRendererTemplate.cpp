#include "GlRendererTemplate.h"
#include <FSystem.h>

using namespace GCube;

GlRendererTemplate::GlRendererTemplate(void)
	: __controlWidth(0)
	, __controlHeight(0)
{
}

GlRendererTemplate::~GlRendererTemplate(void)
{

}

bool
GlRendererTemplate::InitializeGl(void)
{
	lastTick = 0;
	gcube = ApplicationController::SharedInstance();
	gcube->onInit();

	return true;
}

bool
GlRendererTemplate::TerminateGl(void)
{
	return true;
}

bool
GlRendererTemplate::Draw(void)
{
	// 時間差分取得
	long long tick = 0;
	Tizen::System::SystemTime::GetTicks(tick);
	long dt = tick - lastTick;
	float dtf = dt / 1000.0;

	// 描画
	gcube->onUpdate(dtf);
	gcube->onDraw();

	lastTick = tick;

	return true;
}

bool
GlRendererTemplate::Pause(void)
{
	gcube->onPause();

	return true;
}

bool
GlRendererTemplate::Resume(void)
{
	gcube->onResume();

	return true;
}

int
GlRendererTemplate::GetTargetControlWidth(void)
{
	return __controlWidth;
}

int
GlRendererTemplate::GetTargetControlHeight(void)
{
	return __controlHeight;
}

void
GlRendererTemplate::SetTargetControlWidth(int width)
{
	__controlWidth = width;
}

void
GlRendererTemplate::SetTargetControlHeight(int height)
{
	__controlHeight = height;
}
