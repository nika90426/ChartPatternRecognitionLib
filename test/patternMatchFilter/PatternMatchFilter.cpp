#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"
#include "PatternMatchSortFunctor.h"
#include "PatternMatchUniqueFunctor.h"
#include "PatternMatchFilter.h"
#include "PatternMatchFindPredicate.h"

using namespace testHelper;
using namespace patternMatchFilter;

BOOST_AUTO_TEST_CASE( PatternMatchFilter_UniqueStartEndDatePatternMatchFilter )
{

	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex=0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

	ChartSegmentList matchSegments1;
	matchSegments1.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,2))));
	matchSegments1.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(1,3)))); // overlap prev seg by 1
	PatternMatchPtr patternMatch1(new PatternMatch(matchSegments1));
    
    // Same start and end date as patternMatch1, but only 1 segment
  	ChartSegmentList matchSegments2;
	matchSegments2.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,3))));
	PatternMatchPtr patternMatch2(new PatternMatch(matchSegments2));
    
	// Same start date as patternMatch1 and patternMatch2, but different end date
	ChartSegmentList matchSegments3;
	matchSegments3.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,2))));
	matchSegments3.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(1,4)))); // overlap prev seg by 1
	PatternMatchPtr patternMatch3(new PatternMatch(matchSegments3));

    // Different start and end dates
  	ChartSegmentList matchSegments4;
  	matchSegments4.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(1,5))));
	PatternMatchPtr patternMatch4(new PatternMatch(matchSegments4));

	PatternMatchListPtr unfilteredMatches(new PatternMatchList);
	unfilteredMatches->push_back(patternMatch4);
	unfilteredMatches->push_back(patternMatch3);
	unfilteredMatches->push_back(patternMatch1);
	unfilteredMatches->push_back(patternMatch2);

	genPatternMatchListInfo("Unfiltered matches",*unfilteredMatches);

	PatternMatchListPtr testSortFunc(new PatternMatchList);
	testSortFunc->insert(testSortFunc->end(),unfilteredMatches->begin(),unfilteredMatches->end());
	testSortFunc->sort(SortPatternMatchByStartAndEndDate());
	genPatternMatchListInfo("Sorted matches",*testSortFunc);
	PatternMatchList::iterator sortedMatchesIter = testSortFunc->begin();
	verifyPatternMatch("Sorted matches [0]",dateToTime(2013,1,1),dateToTime(2013,3,1),1,*sortedMatchesIter);
	sortedMatchesIter++;
	BOOST_TEST_MESSAGE("Second item of the list should be the same as the first but with one more segment");
	verifyPatternMatch("Sorted matches [1]",dateToTime(2013,1,1),dateToTime(2013,3,1),2,*sortedMatchesIter);
	sortedMatchesIter++;
	BOOST_TEST_MESSAGE("Third item of the list should have an end date further into the future");
	verifyPatternMatch("Sorted matches [2]",dateToTime(2013,1,1),dateToTime(2013,4,1),2,*sortedMatchesIter);
	sortedMatchesIter++;
	BOOST_TEST_MESSAGE("Third item of the list should have both a a start and end date further into the future");
	verifyPatternMatch("Sorted matches [3]",dateToTime(2013,2,1),dateToTime(2013,5,1),1,*sortedMatchesIter);

	PatternMatchListPtr uniqueSorted = patternMatchFilter::filterUniqueStartEndTime(unfilteredMatches);
	BOOST_REQUIRE(uniqueSorted->size() == 3);
	genPatternMatchListInfo("Unique matches",*uniqueSorted);
	PatternMatchList::iterator uniqueMatchesIter = uniqueSorted->begin();
	BOOST_TEST_MESSAGE("The sorted & unique matches exclude the duplicate match with 2 segments, starting on 2013-1-1, and ending on 2013-3-1");
	verifyPatternMatch("Sorted matches [0]",dateToTime(2013,1,1),dateToTime(2013,3,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;
	verifyPatternMatch("Sorted matches [1]",dateToTime(2013,1,1),dateToTime(2013,4,1),2,*uniqueMatchesIter);
	uniqueMatchesIter++;
	verifyPatternMatch("Sorted matches [2]",dateToTime(2013,2,1),dateToTime(2013,5,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;
}


BOOST_AUTO_TEST_CASE( PatternMatchFilter_SortByEndDateThenLength )
{

	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex=0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

	ChartSegmentList matchSegments1;
	matchSegments1.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(1,3))));
	PatternMatchPtr patternMatch1(new PatternMatch(matchSegments1));

  	ChartSegmentList matchSegments2;
	matchSegments2.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,3))));
	PatternMatchPtr patternMatch2(new PatternMatch(matchSegments2));

  	ChartSegmentList matchSegments3;
  	matchSegments3.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(2,4))));
	PatternMatchPtr patternMatch3(new PatternMatch(matchSegments3));

   	ChartSegmentList matchSegments4;
  	matchSegments4.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,4))));
	PatternMatchPtr patternMatch4(new PatternMatch(matchSegments4));


	PatternMatchListPtr unfilteredMatches(new PatternMatchList);
	unfilteredMatches->push_back(patternMatch1);
	unfilteredMatches->push_back(patternMatch2);
	unfilteredMatches->push_back(patternMatch3);
	unfilteredMatches->push_back(patternMatch4);

	PatternMatchListPtr  sortedMatches = patternMatchFilter::sortPatternMatches(
				unfilteredMatches,SortPatternMatchByEndTimeThenLength());

	BOOST_TEST_MESSAGE("Expect sorting by end time/date, then start date");
	genPatternMatchListInfo("Sorted matches",*sortedMatches);
	PatternMatchList::iterator sortedMatchesIter = sortedMatches->begin();
	verifyPatternMatch("Sorted matches [0]",dateToTime(2013,1,1),dateToTime(2013,3,1),1,*sortedMatchesIter);
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [1]",dateToTime(2013,2,1),dateToTime(2013,3,1),1,*sortedMatchesIter);
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [3]",dateToTime(2013,1,1),dateToTime(2013,4,1),1,*sortedMatchesIter);
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [4]",dateToTime(2013,3,1),dateToTime(2013,4,1),1,*sortedMatchesIter);

	PatternMatchListPtr  uniqueMatches = patternMatchFilter::uniquePatternMatches(sortedMatches,SameEndTime());
	genPatternMatchListInfo("Unique matches",*uniqueMatches);
	PatternMatchList::iterator uniqueMatchesIter = uniqueMatches->begin();
	verifyPatternMatch("Unique matches [0]",dateToTime(2013,1,1),dateToTime(2013,3,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;
	verifyPatternMatch("Unique matches [1]",dateToTime(2013,1,1),dateToTime(2013,4,1),1,*uniqueMatchesIter);

}

BOOST_AUTO_TEST_CASE( PatternMatchFilter_SortAndUniqueHighestHighPivot )
{

	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex = 0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++)); // highest high
	testData->push_back(testPeriodVal(2013,4,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,1.0,1,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

	BOOST_TEST_MESSAGE("testSeg: highestHighVal()" << testSeg->highestHighVal());
	BOOST_TEST_MESSAGE("testSeg: highestHighVal().periodTime()" << testSeg->highestHighVal().periodTime());

	ChartSegmentList matchSegments1;
	matchSegments1.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(1,4))));
	PatternMatchPtr patternMatch1(new PatternMatch(matchSegments1));
	BOOST_TEST_MESSAGE("patternMatch1: highest high: " << patternMatch1->highestHigh());
	BOOST_TEST_MESSAGE("patternMatch1: highest high val: " << patternMatch1->highestHighVal());
	BOOST_TEST_MESSAGE("patternMatch1: highest high time: " << patternMatch1->highestHighTime());

	ChartSegmentList matchSegments2;
	matchSegments2.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,4))));
	PatternMatchPtr patternMatch2(new PatternMatch(matchSegments2));
	BOOST_TEST_MESSAGE("patternMatch2: highest high: " << patternMatch2->highestHighVal());
	BOOST_TEST_MESSAGE("patternMatch2: highest high time: " << patternMatch2->highestHighTime());

	ChartSegmentList matchSegments3;
	matchSegments3.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(0,2))));
	PatternMatchPtr patternMatch3(new PatternMatch(matchSegments3));
	BOOST_TEST_MESSAGE("patternMatch3: highest high time: " << patternMatch3->highestHighTime());

	ChartSegmentList matchSegments4;
	matchSegments4.push_back(ChartSegmentPtr(new ChartSegment(testSeg->spliceRange(3,5))));
	PatternMatchPtr patternMatch4(new PatternMatch(matchSegments4));
	BOOST_TEST_MESSAGE("patternMatch4: highest high time: " << patternMatch4->highestHighTime());

	PatternMatchListPtr unfilteredMatches(new PatternMatchList);
	unfilteredMatches->push_back(patternMatch1);
	unfilteredMatches->push_back(patternMatch2);
	unfilteredMatches->push_back(patternMatch3);
	unfilteredMatches->push_back(patternMatch4);

	PatternMatchListPtr  sortedMatches = patternMatchFilter::sortPatternMatches(
				unfilteredMatches,SortPatternMatchByHighestHighTimeThenLength());

	BOOST_TEST_MESSAGE("Sorted highest high pattern matches");
	genPatternMatchListInfo("Sorted matches",*sortedMatches);
	PatternMatchList::iterator sortedMatchesIter = sortedMatches->begin();
	verifyPatternMatch("Sorted matches [0]",dateToTime(2013,1,1),dateToTime(2013,2,1),1,*sortedMatchesIter); // unique highest high
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [1]",dateToTime(2013,1,1),dateToTime(2013,4,1),1,*sortedMatchesIter);
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [2]",dateToTime(2013,2,1),dateToTime(2013,4,1),1,*sortedMatchesIter); // same highest high as [1], but shorter
	sortedMatchesIter++;
	verifyPatternMatch("Sorted matches [3]",dateToTime(2013,4,1),dateToTime(2013,5,1),1,*sortedMatchesIter); // unique highest high
	sortedMatchesIter++;


	PatternMatchListPtr  uniqueMatches = patternMatchFilter::filterUniqueAndLongestHighestHigh(unfilteredMatches);
	genPatternMatchListInfo("Unique highest high pattern matches",*uniqueMatches);
	PatternMatchList::iterator uniqueMatchesIter = uniqueMatches->begin();

	verifyPatternMatch("Unique matches [0]",dateToTime(2013,1,1),dateToTime(2013,2,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;
	verifyPatternMatch("Unique matches [1]",dateToTime(2013,1,1),dateToTime(2013,4,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;
	verifyPatternMatch("Unique matches [2]",dateToTime(2013,4,1),dateToTime(2013,5,1),1,*uniqueMatchesIter);
	uniqueMatchesIter++;


}


BOOST_AUTO_TEST_CASE( PatternMatchFilter_FindFirstLowestLow )
{

	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex = 0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++)); // highest high
	testData->push_back(testPeriodVal(2013,4,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,1.0,1,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

	PatternMatchPtr patternMatch1 = segmentSpiceToPatternMatch(testSeg,1,3);
	BOOST_TEST_MESSAGE("patternMatch1: lowest low: " << patternMatch1->lowestLow());
	BOOST_TEST_MESSAGE("patternMatch1: lowest low time: " << patternMatch1->lowestLowTime());
	BOOST_CHECK(patternMatch1->lowestLowTime() == dateToTime(2013,2,1));

	PatternMatchPtr patternMatch2 = segmentSpiceToPatternMatch(testSeg,2,5);
	BOOST_TEST_MESSAGE("patternMatch2: lowest low: " << patternMatch2->lowestLow());
	BOOST_TEST_MESSAGE("patternMatch2: lowest low time: " << patternMatch2->lowestLowTime());
	BOOST_CHECK(patternMatch2->lowestLowTime() == dateToTime(2013,5,1));

	PatternMatchListPtr patternMatchList(new PatternMatchList());
	patternMatchList->push_back(patternMatch1);
	patternMatchList->push_back(patternMatch2);

	PatternMatchList::iterator lowAfterIter = findFirstPatternMatch(patternMatchList,LowestLowAfterTime(dateToTime(2012,12,1)));
	BOOST_REQUIRE(lowAfterIter != patternMatchList->end());
	BOOST_CHECK((*lowAfterIter)->lowestLowTime() == dateToTime(2013,2,1));

	lowAfterIter = findFirstPatternMatch(patternMatchList,LowestLowAfterTime(dateToTime(2013,3,1)));
	BOOST_REQUIRE(lowAfterIter != patternMatchList->end());
	BOOST_CHECK((*lowAfterIter)->lowestLowTime() == dateToTime(2013,5,1));

	lowAfterIter = findFirstPatternMatch(patternMatchList,LowestLowAfterTime(dateToTime(2013,5,1)));
	BOOST_REQUIRE(lowAfterIter == patternMatchList->end());

}
