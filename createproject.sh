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

# copy assets and main
outpath=$2/$1
rm -rf $outpath
mkdir -p $outpath
cp -r $gcubepath/assets $outpath
cp -r $gcubepath/main $outpath

# copy iOS
iospath='/platforms/iOS'
mkdir -p $outpath$iospath
cp -a $gcubepath$iospath/GCubeTemplate $outpath$iospath/$1
cp -a $gcubepath$iospath/GCube/icon $outpath$iospath/$1/icon
cp -a $gcubepath$iospath/GCube/user $outpath$iospath/$1/user
mkdir -p $outpath$iospath/$1.xcodeproj
sed -e "s/GCubeTemplate/$1/" $gcubepath$iospath/GCubeTemplate.xcodeproj/project.pbxproj | sed -e "s/GCube\/user/$1\/user/" | sed -e "s/GCube\/icon/$1\/icon/" > $outpath$iospath/$1.xcodeproj/project.pbxproj

# copy Android
## copy header
droidpath='/platforms/Android'
cd $gcubepath/engine/
headerpath='../platforms/Android/jni/Headers/'
find . -name "*.h" -print | cpio -pdv $headerpath
cd -
cp -a $gcubepath$droidpath $outpath$droidpath
cd $gcubepath/engine/
rm -rf $headerpath
cd -
## remove
rm -rf $outpath$droidpath/bin/
rm -rf $outpath$droidpath/gen/
rm -rf $outpath$droidpath/obj/
#rm -rf $outpath$droidpath/jni/OpenAL
rm -rf $outpath$droidpath/jni/mk
rm -f $outpath$droidpath/jni/Android.mk
## change project name
mv $outpath$droidpath/jni/Android_template.mk $outpath$droidpath/jni/Android.mk
sed -e "s/GCube/$1/" $gcubepath$droidpath/.project | sed -e "/engine/d" > $outpath$droidpath/.project
sed -e "s/GCube/$1/" $gcubepath$droidpath/.cproject | sed -e "/engine/d"  > $outpath$droidpath/.cproject
sed -e "s/gcube\"/$1\"/" $gcubepath$droidpath/AndroidManifest.xml > $outpath$droidpath/AndroidManifest.xml
activitypath='/src/com/gclue/gcube/MainActivity.java'
sed -e "s/gcube.R/$1.R/" $gcubepath$droidpath$activitypath > $outpath$droidpath/$activitypath
