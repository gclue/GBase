//
//  GCAppDelegate.m
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/23.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#import "GCAppDelegate.h"
#import <GCube.h>

using namespace GCube;

@interface GCAppDelegate () {
	ApplicationController *gcube;
}

@end

@implementation GCAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	gcube = ApplicationController::SharedInstance();
	gcube->onInit();
	
    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	gcube->onPause();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	gcube->onResume();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	gcube->onTerminate();
}

@end
