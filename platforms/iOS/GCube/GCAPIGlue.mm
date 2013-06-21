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

#import "GCDefines.h"

// 言語コードを取得
std::string GCGetLanguage() {
	NSArray *languages = [NSLocale preferredLanguages];
	NSString *lang = [languages objectAtIndex:0];
	return std::string([lang UTF8String]);
}

// リソースを取得
void GCGetResourceData(const char *fileName, std::vector<char>& outData) {
	NSString *nameStr = [NSString stringWithCString:fileName encoding:NSUTF8StringEncoding];
	NSString *pathStr = [[NSBundle mainBundle] pathForResource:[nameStr lastPathComponent] ofType:nil];
	NSData *data = [NSData dataWithContentsOfFile:pathStr];
	const UInt8* bytes = (UInt8 *)[data bytes];
	outData.assign(bytes, bytes+[data length]);
}

// ストレージパスを取得
std::string GCGetStoragePath(GCStorageType type) {
	NSSearchPathDirectory dir;
	switch (type) {
		case GCStorageTypeDocument:
			dir = NSDocumentDirectory;
			break;
		case GCStorageTypeCache:
			dir = NSCachesDirectory;
			break;
	}
	NSArray *paths = NSSearchPathForDirectoriesInDomains(dir, NSUserDomainMask, YES);
	return std::string([[paths[0] stringByAppendingString:@"/"] UTF8String]);
}
