//
//  ApplicationController.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/23.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__ApplicationController__
#define __GCube__ApplicationController__

namespace GCube {

class ApplicationController {
private:
	ApplicationController(void);
	virtual~ApplicationController(void);
	
public:
	static ApplicationController* SharedInstance(void);
	
	virtual void onInit(void);
	virtual void onTerminate(void);
	virtual void onPause(void);
	virtual void onResume(void);
	virtual void onContextChanged(void);
	virtual void onSizeChanged(float width, float height, int orientation);
	virtual void onLowMemory(void);
	
	virtual void onUpdate(float dt);
	virtual void onDraw();
};
	
}

#endif /* defined(__GCube__ApplicationController__) */
