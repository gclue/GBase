#!/bin/sh

if [ $# -ne 2 ]; then
echo "usage:"
echo " createproject.sh [ProjectName] [OutputDir]\n"
exit 1
fi

gcubepath=`dirname $0`

if [ $2 = $gcubepath ]; then
echo "ERROR"
exit 1
fi

outpath=$2/$1
rm -rf $outpath
mkdir -p $outpath
cp -r $gcubepath/assets $outpath
cp -r $gcubepath/main $outpath

iospath='/platforms/iOS'
mkdir -p $outpath$iospath
cp -a $gcubepath$iospath/GCubeTemplate $outpath$iospath/$1
cp -a $gcubepath$iospath/GCube/icon $outpath$iospath/$1/icon
cp -a $gcubepath$iospath/GCube/user $outpath$iospath/$1/user
mkdir -p $outpath$iospath/$1.xcodeproj

sed -e "s/GCubeTemplate/$1/" $gcubepath$iospath/GCubeTemplate.xcodeproj/project.pbxproj | sed -e "s/GCube\/user/$1\/user/" | sed -e "s/GCube\/icon/$1\/icon/" > $outpath$iospath/$1.xcodeproj/project.pbxproj
