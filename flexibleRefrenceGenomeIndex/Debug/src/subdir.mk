################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/index.cpp 

OBJS += \
./src/index.o 

CPP_DEPS += \
./src/index.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/weenkus/workspace/C++/zavrsni/includes -I/home/weenkus/workspace/C++/zavrsni/lib -include/home/weenkus/workspace/C++/zavrsni/includes/typedef.h -include/home/weenkus/workspace/C++/zavrsni/includes/serialization.c -include/home/weenkus/workspace/C++/zavrsni/includes/serialization.h -include/home/weenkus/workspace/C++/zavrsni/includes/utils.c -include/home/weenkus/workspace/C++/zavrsni/includes/waveletTree.c -include/home/weenkus/workspace/C++/zavrsni/includes/waveletTree.h -include/home/weenkus/workspace/C++/zavrsni/includes/utils.h -include/home/weenkus/workspace/C++/zavrsni/includes/bitUtils.h -include/home/weenkus/workspace/C++/zavrsni/includes/def.h -include/home/weenkus/workspace/C++/zavrsni/includes/macros.h -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


