/*
 * PatternMatchValidatorCreationHelper.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#include <PatternMatchValidatorCreationHelper.h>
#include "ANDPatternMatchValidator.h"
#include "PatternMatchValueRef.h"
#include "PeriodValueRef.h"
#include "DoubleRange.h"
#include "DebugLog.h"

namespace patternMatchValidatorCreationHelper
{


PatternMatchValidatorPtr minValValidator(double minVal, const PatternMatchValueRefPtr &comparisonValRef)
{
     ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
     PatternMatchValueRefPtr minValRef(new FixedPatternMatchValueRef(minVal));
    PatternMatchValidatorPtr minValidator(
            new ValueComparisonMatchValidator(comparisonValRef,minValRef,greaterEqualCompare));
    return minValidator;
}

PatternMatchValidatorPtr maxValValidator(double maxVal,const PatternMatchValueRefPtr &comparisonValRef)
{
    ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
    PatternMatchValueRefPtr maxValRef(new FixedPatternMatchValueRef(maxVal));
    PatternMatchValidatorPtr maxValidator(
            new ValueComparisonMatchValidator(comparisonValRef,maxValRef,lessEqualCompare));
    return maxValidator;
}

PatternMatchValidatorPtr valWithinRangeValidator(const DoubleRange &minMaxVal,const PatternMatchValueRefPtr &comparisonValRef)
{
    PatternMatchValidatorPtr minVal = minValValidator(minMaxVal.minVal(),comparisonValRef);
    PatternMatchValidatorPtr maxVal = maxValValidator(minMaxVal.maxVal(),comparisonValRef);

    PatternMatchValidatorList andList;
    andList.push_back(minVal);
    andList.push_back(maxVal);

    PatternMatchValidatorPtr valInRange(new ANDPatternMatchValidator(andList));
    return valInRange;
}



PatternMatchValidatorPtr depthWithinRangeValidator(const DoubleRange &minMaxDepthPerc)
{
	assert(minMaxDepthPerc.positiveVals());
    return valWithinRangeValidator(minMaxDepthPerc, PatternMatchValueRefPtr(new DepthPercentPatternMatchValueRef()));

}

PatternMatchValidatorPtr depthSinceStartWithinRangeValidator(const DoubleRange &minMaxDepthPerc)
{
    assert(minMaxDepthPerc.positiveVals());
    return valWithinRangeValidator(minMaxDepthPerc, PatternMatchValueRefPtr(new DepthPercentFirstHighLowestLowPatternMatchValueRef()));
}

PatternMatchValidatorPtr lowerLowValidator(const PatternMatchPtr &compareWith)
{
	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr compareWithLow(new FixedPatternMatchValueRef(compareWith->lowestLow()));
	PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());
	PatternMatchValidatorPtr lowerLowValidator(
			new ValueComparisonMatchValidator(validationLowRef,compareWithLow,lessEqualCompare));
	return lowerLowValidator;
}

static PatternMatchValueRefPtr lastHighValueRef()
{
	// Reference the high value for the last value in the pattern match.
	PeriodValueRefPtr highValueRef(new HighPeriodValueRef());
	PatternMatchValueRefPtr lastHighRef(new LastPeriodValPatternMatchValueRef(highValueRef));
	return lastHighRef;
}


static PatternMatchValueRefPtr firstHighValueRef()
{
	// Reference the high value for the last value in the pattern match.
	PeriodValueRefPtr highValueRef(new HighPeriodValueRef());
	PatternMatchValueRefPtr firstHighRef(new FirstPeriodValPatternMatchValueRef(highValueRef));
	return firstHighRef;
}

PatternMatchValidatorPtr highestHighBelowLastHigh()
{
    PatternMatchValueRefPtr lastHigh = lastHighValueRef();
    PatternMatchValueRefPtr highestHigh(new HighestHighPatternMatchValueRef());
    ValueComparatorPtr lessThanEqualCompare(new LessThanEqualValueComparator());
    PatternMatchValidatorPtr lessThanLastHigh(
            new ValueComparisonMatchValidator(highestHigh,lastHigh,lessThanEqualCompare));

    return lessThanLastHigh;

}

PatternMatchValidatorPtr highestCloseBelowFirstHigh()
{
    PatternMatchValueRefPtr firstHigh = firstHighValueRef();

    PeriodValueRefPtr closeValRef(new ClosePeriodValueRef());
    PatternMatchValueRefPtr highestClose(new HighestValPatternMatchValueRef(closeValRef));

    ValueComparatorPtr lessThanEqualCompare(new LessThanEqualValueComparator());

    PatternMatchValidatorPtr lessThanFirstHigh(
            new ValueComparisonMatchValidator(highestClose,firstHigh,lessThanEqualCompare));

    return lessThanFirstHigh;

}


PatternMatchValidatorPtr lastHighAboveFixedValue(double thresholdValue)
{
	assert(thresholdValue >= 0.0);
	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());

	PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(thresholdValue));

	PatternMatchValidatorPtr aboveThresholdValidator(
			new ValueComparisonMatchValidator(lastHighValueRef(),thresholdValRef,greaterEqualCompare));
	return aboveThresholdValidator;
}

PatternMatchValidatorPtr lastHighAboveFirstHigh()
{
	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
	PatternMatchValidatorPtr aboveFirstHigh(
			new ValueComparisonMatchValidator(lastHighValueRef(),firstHighValueRef(),greaterEqualCompare));
	return aboveFirstHigh;
}

PatternMatchValidatorPtr lowAbovePercDepthOfOtherPattern(const PatternMatchPtr &otherPattern, double percDepthOther)
{
    ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
    PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());

    assert(percDepthOther >= 0.0);
    assert(percDepthOther <= 1.0);

    double firstHigh = otherPattern->firstValue().high();
    double lowestLow = otherPattern->lowestLow();
    assert(firstHigh >= lowestLow);
    double depth = firstHigh-lowestLow;

    double thresholdValue = firstHigh-(depth * percDepthOther);
    PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(thresholdValue));

    DEBUG_MSG("lowAbovePercDepthOfOtherPattern: low threshold: " << thresholdValue);


    PatternMatchValidatorPtr aboveDepthOther(
            new ValueComparisonMatchValidator(validationLowRef,thresholdValRef,greaterEqualCompare));

    return aboveDepthOther;

}

}
