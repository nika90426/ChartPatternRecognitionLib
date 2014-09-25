#include "WedgeMatchValidationInfo.h"
#include <boost/log/trivial.hpp>
#include <assert.h>

WedgeMatchValidationInfo::WedgeMatchValidationInfo(const PeriodValSegmentPtr &chartVals,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine,
        const PeriodValCltn::iterator &currPerValIter)
: chartVals_(chartVals),
  upperTrendLine_(upperTrendLine),
  lowerTrendLine_(lowerTrendLine),
  currPerValIter_(currPerValIter)
{
}

const PeriodValCltn::iterator WedgeMatchValidationInfo::patternBeginIter() const
{
    return upperTrendLine_->firstValIter();
}


const PeriodValCltn::iterator WedgeMatchValidationInfo::patternEndIter() const
{
    PeriodValCltn::iterator endIter = currPerValIter_;
    assert(endIter != chartVals_->segEnd());
    endIter++;
    return endIter;
}

ChartSegmentPtr WedgeMatchValidationInfo::createWedgeSegment() const
{
    ChartSegmentPtr wedgeSeg(new ChartSegment(chartVals_->perValCltn(),
            patternBeginIter(),currPerValIter_,
            PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
    return wedgeSeg;
}


PatternMatchBreakoutInfoPtr WedgeMatchValidationInfo::upperTrendLineBreakout() const
{
    PeriodValCltn::iterator prevPerValIter = currPerValIter_;
    prevPerValIter--;

    assert(prevPerValIter != chartVals_->segBegin());

    double currXVal = (*currPerValIter_).pseudoXVal();
    double breakoutYVal = upperTrendLine_->segmentEq()->yVal(currXVal);


    if (upperTrendLine_->segmentEq()->belowLine((*prevPerValIter).closeCoord()) &&
            upperTrendLine_->segmentEq()->aboveLine((*currPerValIter_).closeCoord()))
    {
        BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: upper trend line breakout: "
                    << "prev val=" << (*prevPerValIter).closeCoord()
                    << ", curr val=" << (*currPerValIter_).closeCoord()
                    << ", curr period val=" << (*currPerValIter_) << std::endl;
        return PatternMatchBreakoutInfoPtr(new PatternMatchBreakoutInfo(currXVal,breakoutYVal));
    }
    else
    {
        return PatternMatchBreakoutInfoPtr(); // NULL (smart) pointer
    }

}

PatternMatchBreakoutInfoPtr WedgeMatchValidationInfo::lowerTrendLineBreakdown() const
{
    PeriodValCltn::iterator prevPerValIter = currPerValIter_;
    prevPerValIter--;

    assert(prevPerValIter != chartVals_->segBegin());

    double currXVal = (*currPerValIter_).pseudoXVal();
    double breakoutYVal = lowerTrendLine_->segmentEq()->yVal(currXVal);


    if (lowerTrendLine_->segmentEq()->aboveLine((*prevPerValIter).closeCoord()) &&
            lowerTrendLine_->segmentEq()->belowLine((*currPerValIter_).closeCoord()))
    {
        BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: lower trend line breakdown: "
                    << "prev val=" << (*prevPerValIter).closeCoord()
                    << ", curr val=" << (*currPerValIter_).closeCoord()
                    << ", curr period val=" << (*currPerValIter_) << std::endl;
        return PatternMatchBreakoutInfoPtr(new PatternMatchBreakoutInfo(currXVal,breakoutYVal));
    }
    else
    {
        return PatternMatchBreakoutInfoPtr(); // NULL (smart) pointer
    }

}

bool WedgeMatchValidationInfo::incompletePatternMatch() const
{
    // Validate/confirm an incomplete pattern match if the closing coordinate
    // is still within both the upper and lower trendlines, but is also the last
    // coordinate in the chart data.
    if (lowerTrendLine_->segmentEq()->aboveLine((*currPerValIter_).closeCoord()) &&
            upperTrendLine_->segmentEq()->belowLine((*currPerValIter_).closeCoord()))
    {
        if((*currPerValIter_).perValIndex() ==
                chartVals_->perValCltn()->back().perValIndex())
        {
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


