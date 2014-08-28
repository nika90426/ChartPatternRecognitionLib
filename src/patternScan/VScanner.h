/*
 * VScanner.h
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#ifndef VSCANNER_H_
#define VSCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScanner.h"
#include "PatternMatchValidator.h"
#include "CompositePatternMatchValidatorFactory.h"

// Scan for V shaped patterns. This class is a building block
// for double bottoms or other patterns with a V shape.
class VScanner : public PatternScanner {
private:
	CompositePatternMatchValidatorFactory upTrendValidatorFactory_;
    CompositePatternMatchValidatorFactory downTrendValidatorFactory_;

	CompositePatternMatchValidatorFactory overallValidatorFactory_;
	double trendLineMaxDistancePerc_;
    unsigned int minTrendLineSegmentLength_;
    bool validateWithTrendLineValidator_;

    void initValidationConstraints();
public:
	VScanner();
    VScanner(double trendLineMaxDistancePerc, unsigned int minTrendLineSegmentLength, bool validateWithTrendLineValidator);

    CompositePatternMatchValidatorFactory &downTrendValidatorFactory() { return downTrendValidatorFactory_; }
    CompositePatternMatchValidatorFactory &upTrendValidatorFactory() { return upTrendValidatorFactory_; }
	CompositePatternMatchValidatorFactory &overallValidatorFactory() { return overallValidatorFactory_; }

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
	virtual ~VScanner();
};

#endif /* VSCANNER_H_ */
