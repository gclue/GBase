################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AppResourceId.cpp \
../src/GCube.cpp \
../src/GCubeEntry.cpp \
../src/GCubeFrame.cpp \
../src/GlRendererTemplate.cpp 

OBJS += \
./src/AppResourceId.o \
./src/GCube.o \
./src/GCubeEntry.o \
./src/GCubeFrame.o \
./src/GlRendererTemplate.o 

CPP_DEPS += \
./src/AppResourceId.d \
./src/GCube.d \
./src/GCubeEntry.d \
./src/GCubeFrame.d \
./src/GlRendererTemplate.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++ -I"pch" -D_DEBUG -I"/Users/tsuchiya/Documents/workspace/GCube2/platforms/Tizen/inc" -O0 -g3 -Wall -c -fmessage-length=0 -target arm-tizen-linux-gnueabi -gcc-toolchain /Users/tsuchiya/tizen-sdk/tools/smart-build-interface/../arm-linux-gnueabi-gcc-4.5/ -ccc-gcc-name arm-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mtune=cortex-a8 -Wno-gnu -fPIE --sysroot="/Users/tsuchiya/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-device-2.1.native" -I"/Users/tsuchiya/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-device-2.1.native/usr/include/libxml2" -I"/Users/tsuchiya/tizen-sdk/library" -I"/Users/tsuchiya/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-device-2.1.native/usr/include" -I"/Users/tsuchiya/tizen-sdk/platforms/tizen2.1/rootstraps/tizen-device-2.1.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


