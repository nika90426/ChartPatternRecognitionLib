/*
 * PeriodValSegment.h
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#ifndef PERIODVALSEGMENT_H_
#define PERIODVALSEGMENT_H_

#include <boost/shared_ptr.hpp>
#include "PeriodVal.h"

class PeriodValSegment;
typedef boost::shared_ptr<PeriodValSegment> PeriodValSegmentPtr;
typedef std::pair<PeriodValSegmentPtr,PeriodValSegmentPtr> PeriodValSegmentPair;

// PeriodValSegment is a "flyweight" pattern for accessing into and iterating over
// sequences of PeriodVal data. In general, there is only a need to keep one copy
// of the PeriodVal sequence around, but the algorithms for pattern matching will
// extensively segment and iterate over this sequence.
class PeriodValSegment {
private:
	PeriodValCltnPtr perValCltn_;
	PeriodValCltn::iterator segBegin_;
	PeriodValCltn::iterator segEnd_;

	unsigned int startPos_;
	unsigned int endPos_;

	void initSegments(const PeriodValCltn::iterator &segBegin,
			const PeriodValCltn::iterator &segEnd);
public:
	PeriodValSegment(const PeriodValCltnPtr &perValCltn,
			const PeriodValCltn::iterator &segBegin,
			const PeriodValCltn::iterator &segEnd);

	// Construct a segment encompassing all the values
	// in perValCltn
	PeriodValSegment(const PeriodValCltnPtr &perValCltn);

	PeriodValSegmentPtr moveSegBeginToPrev() const;

	const PeriodValCltn::iterator &segBegin() const { return segBegin_; }
	const PeriodValCltn::iterator &segEnd() const { return segEnd_; }

	PeriodValSegmentPair split(unsigned int splitPos) const;

	unsigned int numVals() const;
	const PeriodVal &firstVal() const;
	const PeriodVal &lastVal() const;

	virtual ~PeriodValSegment();
};




#endif /* PERIODVALSEGMENT_H_ */
