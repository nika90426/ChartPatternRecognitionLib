/*
 * PeriodValueRef.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#ifndef PERIODVALUEREF_H_
#define PERIODVALUEREF_H_

#include "PeriodVal.h"

#include <boost/shared_ptr.hpp>


class PeriodValueRef {
public:
	PeriodValueRef();

	virtual double referencedVal(const PeriodVal &periodVal) const = 0;

	virtual ~PeriodValueRef();
};

typedef boost::shared_ptr<PeriodValueRef> PeriodValueRefPtr;

class HighPeriodValueRef: public PeriodValueRef {
public:
	virtual double referencedVal(const PeriodVal &periodVal) const { return periodVal.high(); }
};

class LowPeriodValueRef: public PeriodValueRef {
public:
	virtual double referencedVal(const PeriodVal &periodVal) const { return periodVal.low(); }
};


class TypicalPricePeriodValueRef: public PeriodValueRef {
public:
	virtual double referencedVal(const PeriodVal &periodVal) const { return periodVal.typicalPrice(); }
};

class ClosePeriodValueRef: public PeriodValueRef {
public:
    virtual double referencedVal(const PeriodVal &periodVal) const { return periodVal.close(); }
};



#endif /* PERIODVALUEREF_H_ */
