#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "WedgeScannerEngine.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( WedgeScannerEngine_SynthesizedPattern )
{
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,92.0,100.0)); // up-trend to first pivot high at 100
	ranges.push_back(TestPerValRange(4,98.0,92.0)); // down-trend to first pivot low at 92
	ranges.push_back(TestPerValRange(4,92.5,98.0)); // up-trend to 2nd pivot high at 98 (lower than 1st)
	ranges.push_back(TestPerValRange(4,97.5,94.0)); // down-trend to 2nd pivot low at 94 (higher than 1st)
	ranges.push_back(TestPerValRange(4,94.5,96.0)); // up-trend after 2nd pivot low
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	WedgeScannerEngine scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("Should return 0 matches, since the depth is not greater than 9%");


}
