################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../quoteData/ClosePeriodValueRef.cpp \
../quoteData/PeriodVal.cpp \
../quoteData/PeriodValSegment.cpp \
../quoteData/PeriodValueRef.cpp 

OBJS += \
./quoteData/ClosePeriodValueRef.o \
./quoteData/PeriodVal.o \
./quoteData/PeriodValSegment.o \
./quoteData/PeriodValueRef.o 

CPP_DEPS += \
./quoteData/ClosePeriodValueRef.d \
./quoteData/PeriodVal.d \
./quoteData/PeriodValSegment.d \
./quoteData/PeriodValueRef.d 


# Each subdirectory must supply rules for building sources it contributes
quoteData/%.o: ../quoteData/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


