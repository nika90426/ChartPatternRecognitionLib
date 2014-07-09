#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodVal.h"
#include "PatternScannerEngine.h"
#include "SegmentConstraint.h"
#include "SegmentValsCloseToLinearEq.h"
#include "SegmentListConstraint.h"
#include "SlopeIncreasesConstraint.h"
#include "PatternMatchValidator.h"
#include "EndWithinPercentOfStart.h"
#include "PeriodValSegment.h"
#include "CupScanner.h"
#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722 )
{

	// "typical price" data for SAVE, starting on 2013-07-22,
	// where typical price is (high + low + close)/3.0

	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

	BOOST_TEST_MESSAGE("CupWithHandle_SAVE_20130722");

	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq(3.0));
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart(8.0));

	PatternScannerEngine scanner(segConstraint,segListConstraint,patternMatchValidator);

	TestHelper::genPeriodValSegmentInfo("Cup data",*chartData);

	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	TestHelper::verifyMatchList("CupWithHandle_SAVE_20130722",patternMatches,2);

	PatternMatchPtr thePatternMatch = patternMatches->front();
	BOOST_CHECK(segListConstraint->validSegments(thePatternMatch->segments()) == true);

	TestHelper::verifyPatternMatch("Cup Match",
			ptime(date(2013,7,22)),ptime(date(2013,10,7)),4,thePatternMatch);

}


BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722_CupScanner )
{
	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

	CupScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	TestHelper::verifyMatchList("CupWithHandle_SAVE_20130722_CupScanner",patternMatches,1);

	TestHelper::verifyPatternMatch("Cup Match",
			ptime(date(2013,7,22)),ptime(date(2013,10,7)),3,patternMatches->front());

}
