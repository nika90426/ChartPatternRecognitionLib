#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricWedgePatternMatch.h"
#include "FlatBasePatternMatch.h"
#include "CupPatternMatch.h"

PatternShapeGenerator::PatternShapeGenerator()
{
}

PatternShapePtr PatternShapeGenerator::generateShape(PatternMatch &patternMatch)
{
    patternShape_ = PatternShapePtr(new PatternShape());

    patternMatch.acceptVisitor(*this);

    assert(patternShape_->numShapes() > 0);

    return patternShape_;
}

static PatternShapePoint createTypicalPriceShapePoint(const PeriodVal &perVal)
{
    return PatternShapePoint(perVal.pseudoXVal(),
                             perVal.periodTime(),perVal.typicalPrice());
}

void PatternShapeGenerator::visitVPatternMatch(VPatternMatch &vMatch)
{
    PatternShapePointVectorPtr vShapePoints(new PatternShapePointVector());

    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.firstValue()));
    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.downTrend()->lastValue()));
    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.lastValue()));

    patternShape_->addLineShape(vShapePoints);
}

void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    PatternShapePointVectorPtr cupShapePoints(new PatternShapePointVector());

    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.downTrend()->firstValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->firstValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->lastValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.upTrend()->lastValue()));

    patternShape_->addCurveShape(cupShapePoints);

}

void PatternShapeGenerator::visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &)
{
    // No-op: pattern shape generation handled by visitVPatternMatch
}

void PatternShapeGenerator::visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &)
{
    // No-op: pattern shape generation handled by visitVPatternMatch and visitCupPatternMatch
    // for both the initial base and handle.
}

void PatternShapeGenerator::visitWedgePatternMatch(WedgePatternMatch &wedge)
{
    // The starting point for drawing the upper and lower trendlines is the first value in upper trend line.
    LinearEquationPtr upperTrendLineEq = wedge.upperTrendLine()->segmentEq();
    LinearEquationPtr lowerTrendLineEq = wedge.lowerTrendLine()->segmentEq();

    PeriodValCltn::iterator beginTrendlineShape = wedge.patternBeginIter();
    PeriodValCltn::iterator endTrendlineShape = wedge.interceptEndIter();

    PatternShapePointVectorPtr upperShapePoints(new PatternShapePointVector());
    PatternShapePointVectorPtr lowerShapePoints(new PatternShapePointVector());

    for(PeriodValCltn::iterator drawTrendLineIter = beginTrendlineShape;
        drawTrendLineIter != endTrendlineShape;
        drawTrendLineIter++)
    {
        double xVal = (*drawTrendLineIter).pseudoXVal();

        double upperYVal = upperTrendLineEq->yVal(xVal);
        upperShapePoints->push_back(PatternShapePoint(xVal,
                           (*drawTrendLineIter).periodTime(),upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,
                                       (*drawTrendLineIter).periodTime(),lowerYVal));
    }
    patternShape_->addLineShape(upperShapePoints);
    patternShape_->addLineShape(lowerShapePoints);

}

void PatternShapeGenerator::visitFlatBasePatternMatch(FlatBasePatternMatch &flatBaseMatch)
{
    // The starting point for drawing the upper and lower trendlines is the first value in upper trend line.
    LinearEquationPtr upperTrendLineEq = flatBaseMatch.upperTrendLine()->segmentEq();
    LinearEquationPtr lowerTrendLineEq = flatBaseMatch.lowerTrendLine()->segmentEq();

    PeriodValCltn::iterator beginTrendlineShape = flatBaseMatch.beginMatchIter();
    PeriodValCltn::iterator endTrendlineShape = flatBaseMatch.endMatchIter();

    PatternShapePointVectorPtr upperShapePoints(new PatternShapePointVector());
    PatternShapePointVectorPtr lowerShapePoints(new PatternShapePointVector());

    for(PeriodValCltn::iterator drawTrendLineIter = beginTrendlineShape;
        drawTrendLineIter != endTrendlineShape;
        drawTrendLineIter++)
    {
        double xVal = (*drawTrendLineIter).pseudoXVal();

        double upperYVal = upperTrendLineEq->yVal(xVal);
        upperShapePoints->push_back(PatternShapePoint(xVal,
                           (*drawTrendLineIter).periodTime(),upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,
                                       (*drawTrendLineIter).periodTime(),lowerYVal));
    }
    patternShape_->addLineShape(upperShapePoints);
    patternShape_->addLineShape(lowerShapePoints);

}

void PatternShapeGenerator::visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitFallingWedgePatternMatch(FallingWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitRisingWedgePatternMatch(RisingWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}
