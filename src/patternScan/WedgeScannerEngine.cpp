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

WedgeScannerEngine::WedgeScannerEngine() {
	minPercDistanceToUpperLowerTrendlineIntercept_ = 0.6;
	minPercValsBetweenTrendlines_ = 0.85;
}


PatternMatchListPtr  WedgeScannerEngine::scanWedgePatternMatches(const PeriodValSegmentPtr &chartVals,
		const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	PatternMatchListPtr wedgeMatches(new PatternMatchList());

	// Calculate the intersection between upper and lower trend-lines.
	// This is where the pattern ends.
	if(lowerTrendLine->segmentEq()->slope() != upperTrendLine->segmentEq()->slope())
	{
		// Only continue if the lower and upper trend-line have different slopes.
		// Otherwise they'll never intersect. This being said, a similar scanning
		// algorithm may be appropriate for channels.
		XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: "
				<< " upper trend line: " << upperTrendLine
				<< " lower trend line: " << lowerTrendLine
				<< " intercept " << trendlineIntercept << std::endl;

		// Only continue if the intercept occurs after the first pivot high's "pseudo X" value
		// (i.e., the unique numerical value assigned for each PeriodVal's date). If the intercept
		// is before the first pivot high, then the lines are angled away from each other, and
		// we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
		// at some point, but not here).
		double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();
		if(trendlineIntercept.x() > firstPivotHighXVal)
		{
			// TODO - Besides validating the trendlines intersect after the first pivot high,
			// validate the slopes of the individual trendlines are within the tolerances for
			// an acceptable pattern match (e.g., we don't want both to have a steep negative slope,
			// even if they do intercept).

			double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;
			BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: "
					<< numPeriodsToIntercept << std::endl;


			// The starting point to test for the end of the pattern match occurs after
			// the 2nd pivot high or the 2nd pivot low, whichever has a date farthest in the future.
			PeriodValCltn::iterator endPatternMatchBeginIter =
					upperTrendLine->lastPeriodVal().periodTime() > lowerTrendLine->lastPeriodVal().periodTime()?
							upperTrendLine->perValSegment()->segEnd():lowerTrendLine->perValSegment()->segEnd();

			// Starting at endPatternMatchBeginIter then ending at the x value of the trend line intercept or
			// the last value in chartVals (whichever comes first), test each potential end point for a
			// completed pattern match.
			PeriodValCltn::iterator prevValIter = endPatternMatchBeginIter;
			// endPatternMatchIter is setup based upon the 2nd pivot high or low, so it cannot start at segBegin()
			assert(prevValIter != chartVals->segBegin());
			prevValIter--;
			for(PeriodValCltn::iterator endPatternMatchIter = endPatternMatchBeginIter;
					(endPatternMatchIter != chartVals->segEnd()) && ((*endPatternMatchIter).pseudoXVal() <= trendlineIntercept.x());
					endPatternMatchIter++, prevValIter++)
			{

				// Validate the matching done thus far is far enough along towards the intercept (e.g., 60-70%).
				DoubleRange patternRange(firstPivotHighXVal,trendlineIntercept.x());
				double percentOfPatternLengthCovered = patternRange.percentWithinRange((*endPatternMatchIter).pseudoXVal());
				BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: validating match length vs pattern length: "
							<< patternRange << " %covered=" << percentOfPatternLengthCovered << std::endl;

				if(percentOfPatternLengthCovered >= minPercDistanceToUpperLowerTrendlineIntercept_)
				{

					if(upperTrendLine->segmentEq()->belowLine((*prevValIter).closeCoord()) &&
							upperTrendLine->segmentEq()->aboveLine((*endPatternMatchIter).closeCoord()))
					{

						BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: upper trend line crossover: "
									<< "prev val=" << (*prevValIter).closeCoord()
									<< ", curr val=" << (*endPatternMatchIter).closeCoord()
									<< ", curr period val=" << (*endPatternMatchIter) << std::endl;

						ChartSegmentPtr chartSeg(new ChartSegment(chartVals->perValCltn(),
								upperTrendLine->firstValIter(),endPatternMatchIter,
								PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
						PatternMatchPtr patternMatch(new PatternMatch(chartSeg));

						wedgeMatches->push_back(patternMatch);
						// TODO Based upon minPercValsBetweenTrendlines_, Check that each value and a % of overall values
						// is between the two equations.

					} // if the closing price crosses over the upper trend line (upside break-out)

				} // If the X value for endPatternMatchIter is far enough along towards the upper&lower trend line intercept

			} // For each potential ending to the pattern match

		} // if trendline intercept occurs after the pivot high

	} // If lower and upper trend line have different slopes (i.e., they intersect)

	return wedgeMatches;

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

						PatternMatchListPtr wedgePatternMatches = scanWedgePatternMatches(chartVals,
								upperTrendLine,lowerTrendLine);
						wedgeMatches->insert(wedgeMatches->end(),wedgePatternMatches->begin(),wedgePatternMatches->end());

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time

		} // for each 2nd pivot high (forming the upper trend-line)

	} // for each starting/first pivot high (forming the upper trend-line)

	return wedgeMatches;

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

