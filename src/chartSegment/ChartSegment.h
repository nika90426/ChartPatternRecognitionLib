/*
 * ChartSegment.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef CHARTSEGMENT_H_
#define CHARTSEGMENT_H_

#include <boost/shared_ptr.hpp>

#include "PeriodValSegment.h"
#include "LinearEquation.h"

class ChartSegment {
private:
	PeriodValSegmentPtr segmentVals_;
	LinearEquationPtr segmentEq_;

	double absRelPercentVal(double comparisonVal, double baseVal) const;
public:
	ChartSegment(const PeriodValSegmentPtr &segmentVals);

	const PeriodVal &lastPeriodVal() const;
	const PeriodVal &firstPeriodVal() const;
	double maxRelPercentVsLinearEq() const;
	double slope() const;

	friend std::ostream& operator<<(std::ostream& os, const ChartSegment& chartSeg);


	virtual ~ChartSegment();
};

typedef boost::shared_ptr<ChartSegment> ChartSegmentPtr;
typedef std::vector<ChartSegmentPtr> ChartSegmentList;

#endif /* CHARTSEGMENT_H_ */
