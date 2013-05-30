//
//  GCube.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/30.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef GCube_GCube_h
#define GCube_GCube_h

typedef enum GCDeviceOrientation
{
    GCDeviceOrientationUnknown,
    GCDeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
    GCDeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
    GCDeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
    GCDeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
    GCDeviceOrientationFaceUp,              // Device oriented flat, face up
    GCDeviceOrientationFaceDown             // Device oriented flat, face down
}
GCDeviceOrientation;

#include "ApplicationController.h"
#include "util/Log.h"

#endif
