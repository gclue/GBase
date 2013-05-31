//
//  glsample.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/31.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__glsample__
#define __GCube__glsample__

// Android
#ifdef __GCube_Android__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

// iOS
#ifdef __GCube_iOS__
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#endif

// Tizen
#ifdef __GCube_Tizen__
	#include <gl2.h>
	#include <gl2ext.h>
#endif

GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
void draw();

#endif /* defined(__GCube__glsample__) */
