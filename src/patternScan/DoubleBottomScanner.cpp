/*
 * DoubleBottomScanner.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include <DoubleBottomScanner.h>
#include "VScanner.h"
#include "PatternMatch.h"
#include "PatternMatchValueRef.h"
#include "ValueComparisonMatchValidator.h"

DoubleBottomScanner::DoubleBottomScanner() {
}


PatternMatchValidatorPtr DoubleBottomScanner::rhsLowerLowValidator(const PatternMatchPtr &lhsMatch) const
{
	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr lhsLow(new FixedPatternMatchValueRef(lhsMatch->lowestLow()));
	PatternMatchValueRefPtr rhsLowRef(new LowestLowPatternMatchValueRef());
	PatternMatchValidatorPtr lowerLowValidator(
			new ValueComparisonMatchValidator(rhsLowRef,lhsLow,lessEqualCompare));
	return lowerLowValidator;
}

PatternMatchListPtr DoubleBottomScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	// The 1st up-trend of the double-bottom must no more than 40% below the depth of the
	// 1st down-trend.
	double minPercentOfDowntrend = 40.0;
	VScanner leftVScanner(minPercentOfDowntrend);
	PatternMatchListPtr leftVMatches = leftVScanner.scanPatternMatches(chartVals);

	PatternMatchListPtr dblBottomMatches(new PatternMatchList());

	for(PatternMatchList::const_iterator leftMatchIter = leftVMatches->begin();
			leftMatchIter!=leftVMatches->end();leftMatchIter++)
	{
		PeriodValSegmentPtr valsForRightVScan = (*leftMatchIter)->trailingValsWithLastVal();

		// The RHS of the RHS V must recover 100% of of the LHS of this V. In other words,
		// for the pattern match to be complete (confirmed), the right-most up-trend of the
		// double-bottom must exceed the start of the 2nd down-trend of the double-bottom.
		double rhsVMinRecoverLHSDepth = 0.0; // RHS of RHS V must recover 100% of the depth of the LHS of V
		VScanner rightVScanner(rhsVMinRecoverLHSDepth);

		PatternMatchValidatorPtr rhsLowerLowValid = rhsLowerLowValidator(*leftMatchIter);

		// Filter down the scanned results for the RHS to those RHS V's which have a lower low
		// than the LHS.
		PatternMatchListPtr rightVMatches = PatternMatchValidator::filterMatches(rhsLowerLowValid,
				rightVScanner.scanPatternMatches(valsForRightVScan));

		// TODO - The left and right V should be in proportion to one another (e.g., the left V
		// shouldn't be 5% down while the right V is 25% down).
		// Put the left V and Right V pattern matches together, forming the overall double-bottom pattern.
		PatternMatchListPtr overallMatches = (*leftMatchIter)->appendMatchList(*rightVMatches);

		// TODO - Probably some overall validation is needed here; e.g., the overall
		// depth of the double bottom should not exceed 30%.
		dblBottomMatches->insert(dblBottomMatches->end(),overallMatches->begin(),overallMatches->end());
	} // for each "left V" match.

	return dblBottomMatches;
}

DoubleBottomScanner::~DoubleBottomScanner() {
}

