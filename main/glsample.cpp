//
//  glsample.cpp
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/31.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#include "glsample.h"

const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"void main() {\n"
"  gl_Position = vPosition;\n"
"}\n";


const char gFragmentShader[] =
"precision mediump float;\n"
"void main() {\n"
"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n";


GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &pSource, 0);
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    return shader;
}


GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    return program;
}

void draw() {
    GLuint gProgram = createProgram(gVertexShader, gFragmentShader);
    GLuint gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	
	const GLfloat vertices[] = {
		0.0f,  0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gProgram);
	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
