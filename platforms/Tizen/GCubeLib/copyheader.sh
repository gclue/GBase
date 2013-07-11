#!/bin/sh

## copy headers
cd ../../../engine/
find . -name "*.h" -print | cpio -pdv ../platforms/Tizen/GCube/engine
