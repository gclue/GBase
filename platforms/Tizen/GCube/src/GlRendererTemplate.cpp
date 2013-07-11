/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
