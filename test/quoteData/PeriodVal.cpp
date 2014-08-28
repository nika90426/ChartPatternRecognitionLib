/*
 * PeriodData.cpp
 *
 *  Created on: Jun 23, 2014
 *      Author: sroehling
 */


#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodValueRef.h"

using namespace boost::posix_time;
using namespace boost::gregorian;


#include "PeriodVal.h"

BOOST_AUTO_TEST_CASE( PeriodValTest_WithAdjustedValues )
{
	// Read a valid quotes file and check some of the values
	PeriodValCltnPtr periodData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	BOOST_CHECK(periodData->size()==26);

	PeriodVal firstVal = periodData->front();
    // Note - both the volume and close values are adjusted, based
    // upon the difference between the close and adjusted close.
    BOOST_CHECK_EQUAL(firstVal.volume(),1449323);
    BOOST_CHECK_CLOSE(firstVal.close(),65.76,0.001);

	ClosePeriodValueRef closeRef;
    BOOST_CHECK_CLOSE(closeRef.referencedVal(firstVal),65.76,0.001);

	PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.volume(),3755681);
	BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,2,18)));

	// Verify error conditions are handled by the quote reading code.
	BOOST_CHECK_THROW(PeriodVal::readFromFile("invalidFileName.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MissingHeader.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MissingField.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/CorruptDate.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/CorruptVal.csv"),std::runtime_error);
}

