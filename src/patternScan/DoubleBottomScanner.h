/*
 * DoubleBottomScanner.h
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#ifndef DOUBLEBOTTOMSCANNER_H_
#define DOUBLEBOTTOMSCANNER_H_

#include <PatternScanner.h>
#include "PatternMatchValidator.h"
#include "DoubleRange.h"

class DoubleBottomScanner: public PatternScanner {
private:
	DoubleRange minMaxDepthPerc_;
public:
	DoubleBottomScanner(const DoubleRange &minMaxDepthPerc);
	DoubleBottomScanner();

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
	virtual ~DoubleBottomScanner();
};

#endif /* DOUBLEBOTTOMSCANNER_H_ */
