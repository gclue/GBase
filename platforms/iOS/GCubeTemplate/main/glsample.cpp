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

#include "glsample.h"

const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"attribute vec2 a_texcoord;\n"
"varying vec2 v_texcoord;\n"
"void main() {\n"
"  v_texcoord = a_texcoord;\n"
"  gl_Position = vPosition;\n"
"}\n";


const char gFragmentShader[] =
"precision mediump float;\n"
"uniform sampler2D u_diffuseTexture;\n"
"varying vec2 v_texcoord;\n"
"void main() {\n"
"  gl_FragColor = vec4(texture2D(u_diffuseTexture, v_texcoord.st));\n"
"}\n";


GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &pSource, 0);
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == 0) {
		LOGD("compile error:%s", pSource);
	}
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
	if (linkStatus == 0) {
		LOGD("link error");
	}
	return program;
}

static GLuint gProgram;

void initProgram(std::vector<char> &buf) {
	gProgram = createProgram(gVertexShader, gFragmentShader);
	
	unsigned char* pixels ;
	int w, h, bpp;
	pixels = stbi_load_from_memory((const unsigned char *)&buf[0], buf.size(), &w, &h, &bpp, 4);
	LOGD("s:%d-%d(%d)", w, h, bpp);
	
	GLuint texName = 0;
	glGenTextures(1, &texName);
	if (!texName) {
		LOGE("** ERROR (create texture) **");
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texName);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void draw() {
	GLuint gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	GLuint gvTexHandle = glGetAttribLocation(gProgram, "a_texcoord");
	
	const GLfloat vertices[] = {
		0.0f,  0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};
	const GLfloat texs[] = {
		0.5f,  0.0f,
		0.f,  1.f,
		1.f,  1.f
	};
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gProgram);
	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(gvPositionHandle);
	glVertexAttribPointer(gvTexHandle, 2, GL_FLOAT, GL_FALSE, 0, texs);
	glEnableVertexAttribArray(gvTexHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	int e = glGetError();
	if (e > 0) {
		LOGE("glGetError:%d", e);
	}
}
