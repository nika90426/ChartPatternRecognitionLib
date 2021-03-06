/*
 * PivotHighScanner.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#include <PivotHighScanner.h>
#include "InvertedVScanner.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "PerValCltnSlidingWindow.h"
#include "DebugLog.h"


PivotHighScanner::PivotHighScanner() {
	pivotHighMaxTrendLineDistancePerc_ = 3.0;
}

PivotHighScanner::PivotHighScanner(double maxTrendLineDistancePerc)
: pivotHighMaxTrendLineDistancePerc_(maxTrendLineDistancePerc)
{
    assert(maxTrendLineDistancePerc > 0.0);
}

PatternMatchListPtr PivotHighScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    //The code below is experimental code for matching pivots. Rather than progressively scanning for
    // using the InvertedVScanner, use a sliding window to determine where pivots have taken place.
    PatternMatchListPtr allPivots(new PatternMatchList());

    unsigned int pivotWindowLen = 6;
    if(PerValCltnSlidingWindow::windowFitsWithinRange(pivotWindowLen,chartVals->segBegin(),chartVals->segEnd()))
    {
        PerValCltnSlidingWindow slidingPivotTestWindow(pivotWindowLen,chartVals->segBegin(),chartVals->segEnd());

        while(!slidingPivotTestWindow.windowAtEnd())
        {
            if((slidingPivotTestWindow.middleVal().high() >
                  slidingPivotTestWindow.firstVal().high()) &&
                    (slidingPivotTestWindow.middleVal().high() >
                     slidingPivotTestWindow.lastVal().high())
                   )
             {

                DEBUG_MSG("PivotHighScanner: pivot high: "
                        << "LHS=" << slidingPivotTestWindow.firstVal()
                        << "Middle (pivot)=" << slidingPivotTestWindow.middleVal()
                        << "RHS=" << slidingPivotTestWindow.lastVal());
                ChartSegmentPtr pivotSeg(new ChartSegment(chartVals->perValCltn(),
                        slidingPivotTestWindow.windowFirst(),slidingPivotTestWindow.windowLast(),
                        PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
                PatternMatchPtr pivotMatch(new PatternMatch(pivotSeg));
                allPivots->push_back(pivotMatch);
               }
                slidingPivotTestWindow.advanceWindow();

        }
    }

    PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(allPivots);


    DEBUG_MSG("PivotHighScanner: num pivot highs: " << sortedUniquePivots->size());
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
        DEBUG_MSG("PivotHighScanner: pivot high:"
				<< " time=" << (*matchIter)->highestHighVal().periodTime()
				<< " (psuedo) x val=" << (*matchIter)->highestHighVal().pseudoXVal()
                << ", highest high=" << (*matchIter)->highestHigh());
	}

	return sortedUniquePivots;
}

PeriodValCltnIterListPtr PivotHighScanner::scanPivotHighBeginIters(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr pivotHighMatches = scanPatternMatches(chartVals);

    PeriodValCltnIterListPtr pivotHighIters(new PeriodValCltnIterList());

    for(PatternMatchList::iterator matchIter = pivotHighMatches->begin(); matchIter != pivotHighMatches->end();
        matchIter++)
    {
        pivotHighIters->push_back((*matchIter)->highestHighIter());
    }

    return pivotHighIters;

}


PivotHighScanner::~PivotHighScanner() {
}

