#ifndef _GCUBEFRAME_H_
#define _GCUBEFRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class GCubeFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	GCubeFrame(void);
	virtual ~GCubeFrame(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif  //_GCUBEFRAME_H_
