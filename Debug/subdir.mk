################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Database.cpp \
../File.cpp \
../Host.cpp \
../Main.cpp \
../Network.cpp \
../Share.cpp \
../Sqlite.cpp \
../Watcher.cpp 

OBJS += \
./Database.o \
./File.o \
./Host.o \
./Main.o \
./Network.o \
./Share.o \
./Sqlite.o \
./Watcher.o 

CPP_DEPS += \
./Database.d \
./File.d \
./Host.d \
./Main.d \
./Network.d \
./Share.d \
./Sqlite.d \
./Watcher.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


