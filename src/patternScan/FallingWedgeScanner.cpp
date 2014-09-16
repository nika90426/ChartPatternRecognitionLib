#include "FallingWedgeScanner.h"

#include "FallingWedgePatternMatch.h"

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScannerEngine::DOWNTREND_SLOPE_RANGE,WedgeScannerEngine::DOWNTREND_SLOPE_RANGE)
{
}


PatternMatchPtr FallingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FallingWedgePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
