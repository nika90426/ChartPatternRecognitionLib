/*
 * WedgeScannerEngine.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
#include <WedgeScannerEngine.h>
#include "PeriodValSegment.h"
#include "PatternMatchFilter.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "DoubleRange.h"
#include "PeriodValueRef.h"
#include "PivotLowScanner.h"
#include "PivotHighScanner.h"
#include "Math.h"
#include "UnsignedIntRange.h"

WedgeScannerEngine::WedgeScannerEngine() {
	minPercDistanceToUpperLowerTrendlineIntercept_ = 0.6;
	minPercValsBetweenTrendlines_ = 0.85;
}

double WedgeScannerEngine::numPeriodsToIntercept(const ChartSegmentPtr &upperTrendLine,const ChartSegmentPtr &lowerTrendLine) const
{
	double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();

	XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));
	double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;
	assert(numPeriodsToIntercept > 0.0);

	return numPeriodsToIntercept;
}



bool WedgeScannerEngine::validTrendLines(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	if(lowerTrendLine->segmentEq()->slope() != upperTrendLine->segmentEq()->slope())
	{
		XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));

		// Only continue if the intercept occurs after the first pivot high's "pseudo X" value
		// (i.e., the unique numerical value assigned for each PeriodVal's date). If the intercept
		// is before the first pivot high, then the lines are angled away from each other, and
		// we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
		// at some point, but not here).
		double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();
		if(trendlineIntercept.x() > firstPivotHighXVal)
		{
			BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: "
					<< " upper trend line: " << upperTrendLine
					<< " lower trend line: " << lowerTrendLine
					<< " intercept " << trendlineIntercept << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


unsigned int WedgeScannerEngine::minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));
	double numPeriodsToIntercept = this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine);

	double minPeriods = floor(minPercDistanceToUpperLowerTrendlineIntercept_ * numPeriodsToIntercept);

	return minPeriods;

}

unsigned int WedgeScannerEngine::maxPatternPeriods(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));

	unsigned int maxPeriods = floor(this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine));
	BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: "
			<< maxPeriods << std::endl;

	return maxPeriods;
}

PatternMatchPtr WedgeScannerEngine::findPatternMatch(const PeriodValSegmentPtr &chartVals,
		const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine,
		const PeriodValCltn::iterator &currPerValIter) const
{
	PeriodValCltn::iterator prevPerValIter = currPerValIter;
	prevPerValIter--;

	if (upperTrendLine->segmentEq()->belowLine((*prevPerValIter).closeCoord()) &&
			upperTrendLine->segmentEq()->aboveLine((*currPerValIter).closeCoord()))
	{
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: upper trend line crossover: "
					<< "prev val=" << (*prevPerValIter).closeCoord()
					<< ", curr val=" << (*currPerValIter).closeCoord()
					<< ", curr period val=" << (*currPerValIter) << std::endl;
		ChartSegmentPtr chartSeg(new ChartSegment(chartVals->perValCltn(),
				upperTrendLine->firstValIter(),currPerValIter,
				PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
		return PatternMatchPtr(new PatternMatch(chartSeg));
	}

	return PatternMatchPtr(); // NULL (smart) pointer

}


PatternMatchListPtr WedgeScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr wedgeMatches(new PatternMatchList());

	// Unlike the PatternScannerEngine, this scanner will return pattern matches starting
	// anywhere from the beginning of chartVals to the end.

	// The scanning starts by getting a list of pivot highs and lows within chartVals.
	// These pivots serve as anchor points for drawing candidate trend lines for the
	// pattern matches.
	PatternMatchListPtr pivotHighs = PivotHighScanner().scanPatternMatches(chartVals);
	PatternMatchListPtr pivotLows = PivotLowScanner().scanPatternMatches(chartVals);

	// Each of the pivot highs serves as a potential starting point for the pattern match.
	for(PatternMatchList::iterator startMatchPivotHighIter = pivotHighs->begin();
			startMatchPivotHighIter != pivotHighs->end(); startMatchPivotHighIter++)
	{

		PeriodValCltn::iterator firstPivotHighIter = (*startMatchPivotHighIter)->highestHighIter();

		PatternMatchList::iterator remainingPivotsBegin = startMatchPivotHighIter;
		remainingPivotsBegin++;
		// The remaining pivot highs are candidates for drawing trend lines to form the pattern.
		for(PatternMatchList::iterator trendLineEndPointPivotHighIter = remainingPivotsBegin;
				trendLineEndPointPivotHighIter != pivotHighs->end(); trendLineEndPointPivotHighIter++)
		{
			PeriodValCltn::iterator secondPivotHighIter = (*trendLineEndPointPivotHighIter)->highestHighIter();
			// Draw the trend line from (*startMatchPivotHighIter) to (*trendLineEndPointPivotHighIter).
			// This trend line is only valid for purposes of continued matching the wedge pattern if it has
			// a negative slope.

			// Find the first pivot low which starts after the starting pivot high. This
			// first pivot low serves as the candidate starting point to draw the corresponding
			// lower trend line.
			PatternMatchList::iterator pivotLowBeginAfterPivotHighStart =
					patternMatchFilter::findFirstPatternMatch(pivotLows,
							LowestLowAfterTime((*startMatchPivotHighIter)->highestHighTime()));
			if(pivotLowBeginAfterPivotHighStart != pivotLows->end())
			{
				for(PatternMatchList::iterator startPivotLowIter = pivotLowBeginAfterPivotHighStart;
						startPivotLowIter != pivotLows->end(); startPivotLowIter++)
				{
					PeriodValCltn::iterator firstPivotLowIter = (*startPivotLowIter)->lowestLowIter();

					// TODO - Test if depth from the pivot high to pivot low satisfies the depth criterion.
					PatternMatchList::iterator remainingPivotLowsBegin = startPivotLowIter;
					remainingPivotLowsBegin++;
					for(PatternMatchList::iterator trendLineEndPointPivotLowIter = remainingPivotLowsBegin;
							trendLineEndPointPivotLowIter != pivotLows->end(); trendLineEndPointPivotLowIter++)
					{
						// Create Linear equations defining the upper and lower trend-lines
						PeriodValCltn::iterator secondPivotLowIter = (*trendLineEndPointPivotLowIter)->lowestLowIter();

						ChartSegmentPtr upperTrendLine(new ChartSegment(chartVals->perValCltn(),
								firstPivotHighIter,secondPivotHighIter,
								PeriodValueRefPtr(new HighPeriodValueRef())));
						ChartSegmentPtr lowerTrendLine(new ChartSegment(chartVals->perValCltn(),
								firstPivotLowIter,secondPivotLowIter,
								PeriodValueRefPtr(new LowPeriodValueRef)));

						if(validTrendLines(upperTrendLine,lowerTrendLine))
						{
							// The starting point to test for the end of the pattern match occurs after
							// the 2nd pivot high or the 2nd pivot low, whichever has a date farthest in the future.
							PeriodValCltn::iterator endPatternMatchBeginIter =
									upperTrendLine->lastPeriodVal().periodTime() > lowerTrendLine->lastPeriodVal().periodTime()?
											upperTrendLine->perValSegment()->segEnd():lowerTrendLine->perValSegment()->segEnd();

							assert((*endPatternMatchBeginIter).perValIndex() > (*firstPivotHighIter).perValIndex());
							unsigned int currNumPeriods = (*endPatternMatchBeginIter).perValIndex() -
									(*firstPivotHighIter).perValIndex();

							UnsignedIntRange validPeriodRange( minPatternPeriods(upperTrendLine,lowerTrendLine),
									maxPatternPeriods(upperTrendLine,lowerTrendLine));
							for(PeriodValCltn::iterator endPatternMatchIter = endPatternMatchBeginIter;
									(endPatternMatchIter != chartVals->segEnd()) && (currNumPeriods<=validPeriodRange.maxVal());
									endPatternMatchIter++, currNumPeriods++)
							{
								if(validPeriodRange.valueWithinRange(currNumPeriods))
								{
									PatternMatchPtr foundPatternMatch = findPatternMatch(chartVals,
											upperTrendLine,lowerTrendLine,endPatternMatchIter);
									if(foundPatternMatch)
									{
										wedgeMatches->push_back(foundPatternMatch);
									}
								} // If the period length is within the valid range

								assert(((*endPatternMatchIter).perValIndex()-(*firstPivotHighIter).perValIndex())==currNumPeriods);
							} // for each potential ending to the match

						} // if valid trend line

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time

		} // for each 2nd pivot high (forming the upper trend-line)

	} // for each starting/first pivot high (forming the upper trend-line)

	return wedgeMatches;

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

