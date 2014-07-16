################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternScan/AscendingBasePullbackScanner.cpp \
../patternScan/AscendingBaseScanner.cpp \
../patternScan/CupScanner.cpp \
../patternScan/DoubleBottomScanner.cpp \
../patternScan/InvertedVScanner.cpp \
../patternScan/PatternScannerEngine.cpp \
../patternScan/ScannerHelper.cpp \
../patternScan/TrendLineScanner.cpp \
../patternScan/VScanner.cpp 

OBJS += \
./patternScan/AscendingBasePullbackScanner.o \
./patternScan/AscendingBaseScanner.o \
./patternScan/CupScanner.o \
./patternScan/DoubleBottomScanner.o \
./patternScan/InvertedVScanner.o \
./patternScan/PatternScannerEngine.o \
./patternScan/ScannerHelper.o \
./patternScan/TrendLineScanner.o \
./patternScan/VScanner.o 

CPP_DEPS += \
./patternScan/AscendingBasePullbackScanner.d \
./patternScan/AscendingBaseScanner.d \
./patternScan/CupScanner.d \
./patternScan/DoubleBottomScanner.d \
./patternScan/InvertedVScanner.d \
./patternScan/PatternScannerEngine.d \
./patternScan/ScannerHelper.d \
./patternScan/TrendLineScanner.d \
./patternScan/VScanner.d 


# Each subdirectory must supply rules for building sources it contributes
patternScan/%.o: ../patternScan/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchValidator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


