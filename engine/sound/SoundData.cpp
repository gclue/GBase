//
//  SoundData.cpp
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/06/27.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#include "SoundData.h"
#include "GCube.h"

namespace GCube {

bool SoundData::loadOgg(const char *fileName) {
	
	// リソース読み込み
	std::vector<char> buf;
	ApplicationController *ctr = ApplicationController::SharedInstance();
	ctr->getResource(fileName, buf);
	
	// Oggデコード
	int channels;
	dataSize = stb_vorbis_decode_memory((unsigned char *)&buf[0], buf.size(), &channels, &sampleRate, &data);
	fileType = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
	
	// stb_vorbis_decode_memoryが返すのはframe数、1frame2byteなので。
	dataSize *= sizeof(*data);

	return dataSize>0;
};

}