/*
 * TestHelper.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>


#include "TestHelper.h"
#include "PatternMatch.h"
#include "TimeHelper.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

namespace testHelper
{

void genPatternMatchInfo(const std::string &prefix, const PatternMatchPtr &patternMatch)
{
	BOOST_TEST_MESSAGE(prefix << ": pattern match: "
			<< "start = " << TimeHelper::formatDate(patternMatch->firstValue().periodTime())
			<< ", end = " << TimeHelper::formatDate(patternMatch->lastValue().periodTime())
			<< ", num segments = " << patternMatch->numSegments()
			<< ", depth % = " << patternMatch->depthPercent()
		<< ", last close = " << patternMatch->lastValue().close());

}

void genPatternMatchListInfo(const std::string prefix, const PatternMatchList &matchList)
{
	BOOST_TEST_MESSAGE(prefix << ": number of pattern matches = " << matchList.size());

	for(PatternMatchList::const_iterator matchIter = matchList.begin();
			matchIter != matchList.end(); matchIter++)
	{
		genPatternMatchInfo(prefix,*matchIter);
	}

}

void verifyMatchList(const std::string prefix,
		const PatternMatchListPtr &matchList, unsigned int expectedNumMatches)
{
	genPatternMatchListInfo(prefix,*matchList);
	BOOST_REQUIRE(matchList->size() == expectedNumMatches);
}

void genPeriodValSegmentInfo(const std::string prefix, const PeriodValSegment &perSegment)
{
	for(PeriodValCltn::iterator segIter = perSegment.segBegin(); segIter != perSegment.segEnd(); segIter++)
	{
		BOOST_TEST_MESSAGE(prefix << ": period val: "
				<< TimeHelper::formatDate((*segIter).periodTime())
				<< ",c=" << (*segIter).close()
		);
	}
}

void verifyPatternMatch(const std::string &prefix,
		const boost::posix_time::ptime &expectedStart,
		const boost::posix_time::ptime &expectedEnd, unsigned int expectedSegments,
		const PatternMatchPtr &patternMatch)
{

	BOOST_TEST_MESSAGE(prefix << ": number of segments= " << patternMatch->numSegments());
	BOOST_CHECK(patternMatch->numSegments() == expectedSegments);

	PeriodVal firstVal = patternMatch->firstValue();
	PeriodVal lastVal = patternMatch->lastValue();

	BOOST_TEST_MESSAGE(prefix << ": first period value: " << firstVal);
	BOOST_TEST_MESSAGE(prefix << ": last period value: " << lastVal);
	genPatternMatchInfo(prefix,patternMatch);

	BOOST_CHECK(expectedStart == firstVal.periodTime());
	BOOST_CHECK(expectedEnd == lastVal.periodTime());

}

void verifyPatternMatch(const std::string &prefix,
		const boost::posix_time::ptime &expectedStart,
		const boost::posix_time::ptime &expectedEnd, unsigned int expectedSegments,
		const PatternMatchListPtr &patternMatchList, unsigned int matchIndex)
{
	BOOST_REQUIRE(matchIndex < patternMatchList->size());
	PatternMatchList::iterator matchIter = patternMatchList->begin();
	unsigned int currIndex = 0;
	while(currIndex < matchIndex)
	{
		currIndex++;
		matchIter++;
	}
	assert(matchIter != patternMatchList->end());
	std::string extendedPrefix = prefix;
	extendedPrefix.append(" [");
	extendedPrefix.append(boost::lexical_cast<std::string>(matchIndex));
	extendedPrefix.append("]");
	verifyPatternMatch(extendedPrefix,expectedStart,expectedEnd,expectedSegments,*matchIter);
}

boost::posix_time::ptime dateToTime(unsigned int year, unsigned int month, unsigned int day)
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	return ptime(date(year,month,day));
}


PeriodVal testPeriodVal(unsigned int year, unsigned int month, unsigned int day, double val, unsigned int vol, unsigned int perValIndex)
{
	ptime perTime(date(year,month,day));
	PeriodVal perVal(perTime,val,val,val,val,vol,perValIndex);
	return perVal;
}

PeriodValSegmentPtr synthesizePeriodValSegment(const boost::gregorian::date &startDate,
		const TestPerValRangeList &ranges)
{
	using namespace boost::gregorian;
	using namespace boost::posix_time;

	PeriodValCltnPtr perVals(new PeriodValCltn());

	assert(ranges.size() > 0);

	day_iterator dayIncr(startDate, 1);

	for(TestPerValRangeList::const_iterator rangeIter = ranges.begin();
			rangeIter != ranges.end(); rangeIter++)
	{
		double valIncr = (*rangeIter).valIncr();
		double currVal = (*rangeIter).startVal();

		for(unsigned int dayCount = 0; dayCount < (*rangeIter).numVals(); dayCount++)
		{
			ptime currTime(*dayIncr);

			BOOST_TEST_MESSAGE("Synthesized PeriodValSegment date: " << currTime << " val: " << currVal);
			unsigned int dummyPerValIndexForIndexForReAssignment = 0;
			perVals->push_back(PeriodVal(currTime,currVal,currVal,currVal,currVal,currVal,dummyPerValIndexForIndexForReAssignment));

			currVal += valIncr;
			++dayIncr;
		}

	}

	PeriodVal::reAssignIndices(*perVals);

	return PeriodValSegmentPtr(new PeriodValSegment(perVals));

}

PatternMatchPtr segmentSpiceToPatternMatch(const PeriodValSegmentPtr &seg, unsigned int beginRange, unsigned int endRange)
{
	assert(endRange >= beginRange);
	ChartSegmentList matchSegments;
	matchSegments.push_back(ChartSegmentPtr(new ChartSegment(seg->spliceRange(beginRange,endRange))));
	PatternMatchPtr patternMatch(new PatternMatch(matchSegments));
	return patternMatch;
}

PatternMatchPtr segmentToPatternMatch(const PeriodValSegmentPtr &seg)
{
    ChartSegmentList matchSegments;
    matchSegments.push_back(ChartSegmentPtr(new ChartSegment(seg)));
    PatternMatchPtr patternMatch(new PatternMatch(matchSegments));
    return patternMatch;
}

PatternMatchPtr synthesizePatternMatch(const boost::gregorian::date &startDate,
                                       const TestPerValRangeList &ranges)
{
    PeriodValSegmentPtr perValSeg = synthesizePeriodValSegment(startDate,ranges);
    return segmentToPatternMatch(perValSeg);
}

void genPivotLowInfo(const PatternMatchListPtr &pivotLows)
{
    for(PatternMatchList::iterator pivLowIter = pivotLows->begin();
            pivLowIter != pivotLows->end(); pivLowIter++)
    {
        BOOST_TEST_MESSAGE("PivotLowScanner: pivot low:"
                << " time=" << (*pivLowIter)->lowestLowVal().periodTime()
                << " (psuedo) x val=" << (*pivLowIter)->lowestLowVal().pseudoXVal()
                << ", lowest low=" << (*pivLowIter)->lowestLow());
    }

}

void genPivotHighInfo(const PatternMatchListPtr &pivotHighs)
{
    // Each of the pivot highs serves as a potential starting point for the pattern match.
    for(PatternMatchList::iterator pivHighIter = pivotHighs->begin();
            pivHighIter != pivotHighs->end(); pivHighIter++)
    {
        BOOST_TEST_MESSAGE("PivotHighScanner: pivot high:"
                << " time=" << (*pivHighIter)->highestHighVal().periodTime()
                << " (psuedo) x val=" << (*pivHighIter)->highestHighVal().pseudoXVal()
                << ", highest high=" << (*pivHighIter)->highestHigh());
    }

}



} // namespace TestHelper
