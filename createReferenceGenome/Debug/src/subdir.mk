################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/coder.cpp 

OBJS += \
./src/coder.o 

CPP_DEPS += \
./src/coder.d 


# Each subdirectory must supply rules for building sources it contributes
src/coder.o: ../src/coder.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/weenkus/workspace/C++/zavrsni/includes -include/home/weenkus/workspace/C++/zavrsni/includes/def.h -include/home/weenkus/workspace/C++/zavrsni/includes/bitUtils.c -include/home/weenkus/workspace/C++/zavrsni/includes/bitUtils.h -include/home/weenkus/workspace/C++/zavrsni/includes/macros.h -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/coder.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


