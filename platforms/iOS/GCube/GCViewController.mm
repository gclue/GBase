//
//  GCViewController.m
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/23.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#import "GCViewController.h"
#import <GCube.h>
#import <GCubeConfig.h>

using namespace GCube;

@interface GCViewController () {
	ApplicationController *gcube;
}
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation GCViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
	gcube = ApplicationController::SharedInstance();
	
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	self.preferredFramesPerSecond = __GCube_FrameRate__;

	NSAssert(self.context, @"Failed to create ES context");
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [EAGLContext setCurrentContext:self.context];
	
	gcube->onContextChanged();
	gcube->onSizeChanged(view.bounds.size.width, view.bounds.size.height, (GCDeviceOrientation)self.interfaceOrientation);
}

- (void)dealloc
{    
    [EAGLContext setCurrentContext:self.context];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
	gcube->onUpdate(self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	gcube->onDraw();
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	gcube->onSizeChanged(self.view.bounds.size.width, self.view.bounds.size.height, (GCDeviceOrientation)self.interfaceOrientation);
}

@end
