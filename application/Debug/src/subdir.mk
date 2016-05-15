################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ControlWindow.cpp \
../src/MainWindow.cpp \
../src/ObjectFinder.cpp \
../src/ProjectionWindow.cpp \
../src/SmartPool.cpp 

OBJS += \
./src/ControlWindow.o \
./src/MainWindow.o \
./src/ObjectFinder.o \
./src/ProjectionWindow.o \
./src/SmartPool.o 

CPP_DEPS += \
./src/ControlWindow.d \
./src/MainWindow.d \
./src/ObjectFinder.d \
./src/ProjectionWindow.d \
./src/SmartPool.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


