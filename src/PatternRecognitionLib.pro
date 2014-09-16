#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T09:28:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = PatternRecognitionLib
TEMPLATE = lib
CONFIG += staticlib

# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11

SOURCES += \
    chartSegment/ANDSegmentConstraint.cpp \
    chartSegment/ChartSegment.cpp \
    chartSegment/SegmentValsCloseToLinearEq.cpp \
    chartSegment/SlopeWithinRangeConstraint.cpp \
    chartSegmentList/AnySegmentListValidConstraint.cpp \
    chartSegmentList/SegmentListConstraint.cpp \
    chartSegmentList/SlopeIncreasesConstraint.cpp \
    date/DateValue.cpp \
    date/TimeHelper.cpp \
    math/DoubleRange.cpp \
    math/LinearEquation.cpp \
    math/MathHelper.cpp \
    math/UnsignedIntRange.cpp \
    math/XYCoord.cpp \
    patternMatch/PatternMatch.cpp \
    patternMatch/PatternMatchVisitor.cpp \
    patternMatchFilter/PatternMatchFilter.cpp \
    patternMatchFilter/PatternMatchFindPredicate.cpp \
    patternMatchFilter/PatternMatchSortFunctor.cpp \
    patternMatchFilter/PatternMatchUniqueFunctor.cpp \
    patternMatchValidator/ANDPatternMatchValidator.cpp \
    patternMatchValidator/AnyPatternMatchValidator.cpp \
    patternMatchValidator/CompositePatternMatchValidatorFactory.cpp \
    patternMatchValidator/EndWithinPercentOfStart.cpp \
    patternMatchValidator/HigherLowPatternMatchValidatorFactory.cpp \
    patternMatchValidator/LastValueAbovePointValue.cpp \
    patternMatchValidator/ORPatternMatchValidator.cpp \
    patternMatchValidator/PatternLengthAtLeast.cpp \
    patternMatchValidator/PatternLengthLessThanEqual.cpp \
    patternMatchValidator/PatternMatchValidator.cpp \
    patternMatchValidator/PatternMatchValidatorCreationHelper.cpp \
    patternMatchValidator/PatternMatchValidatorFactory.cpp \
    patternMatchValidator/PatternMatchValueRef.cpp \
    patternMatchValidator/RecoverPercentOfDepth.cpp \
    patternMatchValidator/StaticPatternMatchValidatorFactory.cpp \
    patternMatchValidator/ValueComparisonMatchValidator.cpp \
    patternScan/AscendingBasePullbackScanner.cpp \
    patternScan/AscendingBaseScanner.cpp \
    patternScan/CupScanner.cpp \
    patternScan/DoubleBottomScanner.cpp \
    patternScan/InvertedVScanner.cpp \
    patternScan/MultiPatternScanner.cpp \
    patternScan/PatternScannerEngine.cpp \
    patternScan/PivotHighScanner.cpp \
    patternScan/PivotLowScanner.cpp \
    patternScan/ScannerHelper.cpp \
    patternScan/TrendLineScanner.cpp \
    patternScan/VScanner.cpp \
    patternScan/WedgeScannerEngine.cpp \
    quoteData/PeriodVal.cpp \
    quoteData/PeriodValSegment.cpp \
    quoteData/PeriodValueRef.cpp \
    patternMatch/VPatternMatch.cpp \
    patternShape/PatternShape.cpp \
    patternShape/PatternShapeGenerator.cpp \
    patternMatch/DoubleBottomPatternMatch.cpp \
    patternShape/PatternShapePoint.cpp \
    quoteData/PerValCltnSlidingWindow.cpp \
    patternMatch/CupPatternMatch.cpp \
    patternMatchValidator/SecondPeriodValuePivotsLower.cpp \
    patternMatchValidator/HighestHighLessThanFirstHigh.cpp \
    patternMatchValidator/LowerHighPatternMatchValidatorFactory.cpp \
    patternMatchValidator/LowestLowGreaterThanLastLow.cpp \
    patternMatchValidator/PrevPatternDepthThreshold.cpp \
    patternMatchValidator/ValuesCloseToTrendlineValidator.cpp \
    patternScan/SingleSegmentPatternScannerEngine.cpp \
    patternMatchValidator/PatternSegmentValsCloseToLinearEq.cpp \
    patternMatchValidator/PatternSlopeWithinRange.cpp \
    patternScan/WedgeMatchValidationInfo.cpp \
    patternMatch/PatternMatchBreakoutInfo.cpp \
    patternScan/CupWithHandleScanner.cpp \
    patternMatch/CupWithHandlePatternMatch.cpp \
    patternMatchValidator/BreakoutAboveFirstHighValidatorFactory.cpp \
    patternMatchValidator/CloseWithinPercentOfDepthFromFirstHighValidator.cpp \
    patternMatchValidator/PercentIntersectingPatternLineValidator.cpp \
    patternScan/CupWithoutHandleScanner.cpp \
    patternMatch/CupWithoutHandlePatternMatch.cpp \
    patternMatchValidator/PrevPatternValueRatioValidatorFactory.cpp \
    patternMatchValidator/PrevPatternValueCompareValidatorFactory.cpp \
    patternShape/PatternShapeCurveGenerator.cpp \
    patternScan/TriangleScanner.cpp \
    patternScan/SymetricTriangleScanner.cpp \
    patternScan/AscendingTriangleScanner.cpp \
    patternScan/DescendingTriangleScanner.cpp \
    patternMatch/SymetricTrianglePatternMatch.cpp \
    patternMatch/AscendingTrianglePatternMatch.cpp \
    patternMatch/DescendingTrianglePatternMatch.cpp \
    patternScan/RectangleScanner.cpp \
    patternMatch/RectanglePatternMatch.cpp \
    patternMatch/TrianglePatternMatch.cpp \
    patternScan/WedgeScanner.cpp \
    patternScan/RisingWedgeScanner.cpp \
    patternMatch/WedgePatternMatch.cpp \
    patternMatch/RisingWedgePatternMatch.cpp \
    patternScan/FallingWedgeScanner.cpp \
    patternMatch/FallingWedgePatternMatch.cpp

INCLUDEPATH += segmentContraint\
    chartSegment\
    chartSegmentList\
    date\
    math\
    patternMatch\
    patternMatchFilter\
    patternMatchValidator\
    patternScan\
    patternShape\
    quoteData

# Include the boost headers using the -system flag, suppressing warnigns
# from these files.
macx: QMAKE_CXXFLAGS += -isystem /usr/local/boost156/include
DEFINES += BOOST_ALL_DYN_LINK
DEFINES += BOOST_LOG_DYN_LINK

HEADERS += patternrecognitionlib.h \
    chartSegment/ANDSegmentConstraint.h \
    chartSegment/ChartSegment.h \
    chartSegment/SegmentConstraint.h \
    chartSegment/SegmentValsCloseToLinearEq.h \
    chartSegment/SlopeWithinRangeConstraint.h \
    chartSegmentList/AnySegmentListValidConstraint.h \
    chartSegmentList/SegmentListConstraint.h \
    chartSegmentList/SlopeIncreasesConstraint.h \
    date/DateValue.h \
    date/TimeHelper.h \
    math/DoubleRange.h \
    math/LinearEquation.h \
    math/MathHelper.h \
    math/UnsignedIntRange.h \
    math/ValueComparator.h \
    math/XYCoord.h \
    patternMatch/PatternMatch.h \
    patternMatch/PatternMatchVisitor.h \
    patternMatchFilter/PatternMatchFilter.h \
    patternMatchFilter/PatternMatchFindPredicate.h \
    patternMatchFilter/PatternMatchSortFunctor.h \
    patternMatchFilter/PatternMatchUniqueFunctor.h \
    patternMatchValidator/ANDPatternMatchValidator.h \
    patternMatchValidator/AnyPatternMatchValidator.h \
    patternMatchValidator/CompositePatternMatchValidatorFactory.h \
    patternMatchValidator/EndWithinPercentOfStart.h \
    patternMatchValidator/HigherLowPatternMatchValidatorFactory.h \
    patternMatchValidator/LastValueAbovePointValue.h \
    patternMatchValidator/ORPatternMatchValidator.h \
    patternMatchValidator/PatternLengthAtLeast.h \
    patternMatchValidator/PatternLengthLessThanEqual.h \
    patternMatchValidator/PatternMatchValidator.h \
    patternMatchValidator/PatternMatchValidatorCreationHelper.h \
    patternMatchValidator/PatternMatchValidatorFactory.h \
    patternMatchValidator/PatternMatchValueRef.h \
    patternMatchValidator/RecoverPercentOfDepth.h \
    patternMatchValidator/StaticPatternMatchValidatorFactory.h \
    patternMatchValidator/ValueComparisonMatchValidator.h \
    patternScan/AscendingBasePullbackScanner.h \
    patternScan/AscendingBaseScanner.h \
    patternScan/CupScanner.h \
    patternScan/DoubleBottomScanner.h \
    patternScan/InvertedVScanner.h \
    patternScan/MultiPatternScanner.h \
    patternScan/PatternScanner.h \
    patternScan/PatternScannerEngine.h \
    patternScan/PivotHighScanner.h \
    patternScan/PivotLowScanner.h \
    patternScan/ScannerHelper.h \
    patternScan/TrendLineScanner.h \
    patternScan/VScanner.h \
    patternScan/WedgeScannerEngine.h \
    quoteData/PeriodVal.h \
    quoteData/PeriodValSegment.h \
    quoteData/PeriodValueRef.h \
    patternMatch/VPatternMatch.h \
    patternShape/PatternShape.h \
    patternShape/PatternShapeGenerator.h \
    patternMatch/DoubleBottomPatternMatch.h \
    patternShape/PatternShapePoint.h \
    quoteData/PerValCltnSlidingWindow.h \
    patternMatch/CupPatternMatch.h \
    patternMatchValidator/SecondPeriodValuePivotsLower.h \
    patternMatchValidator/HighestHighLessThanFirstHigh.h \
    patternMatchValidator/LowerHighPatternMatchValidatorFactory.h \
    patternMatchValidator/LowestLowGreaterThanLastLow.h \
    patternMatchValidator/PrevPatternDepthThreshold.h \
    patternMatchValidator/ValuesCloseToTrendlineValidator.h \
    patternScan/SingleSegmentPatternScannerEngine.h \
    patternMatchValidator/PatternSegmentValsCloseToLinearEq.h \
    patternMatchValidator/PatternSlopeWithinRange.h \
    patternScan/WedgeMatchValidationInfo.h \
    patternMatch/PatternMatchBreakoutInfo.h \
    patternScan/CupWithHandleScanner.h \
    patternMatch/CupWithHandlePatternMatch.h \
    patternMatchValidator/BreakoutAboveFirstHighValidatorFactory.h \
    patternMatchValidator/CloseWithinPercentOfDepthFromFirstHighValidator.h \
    patternMatchValidator/PercentIntersectingPatternLineValidator.h \
    patternScan/CupWithoutHandleScanner.h \
    patternMatch/CupWithoutHandlePatternMatch.h \
    patternMatchValidator/PrevPatternValueRatioValidatorFactory.h \
    patternMatchValidator/PrevPatternValueCompareValidatorFactory.h \
    patternShape/PatternShapeCurveGenerator.h \
    patternScan/TriangleScanner.h \
    patternScan/SymetricTriangleScanner.h \
    patternScan/AscendingTriangleScanner.h \
    patternScan/DescendingTriangleScanner.h \
    patternMatch/SymetricTrianglePatternMatch.h \
    patternMatch/AscendingTrianglePatternMatch.h \
    patternMatch/DescendingTrianglePatternMatch.h \
    patternScan/RectangleScanner.h \
    patternMatch/RectanglePatternMatch.h \
    patternMatch/TrianglePatternMatch.h \
    patternScan/WedgeScanner.h \
    patternScan/RisingWedgeScanner.h \
    patternMatch/WedgePatternMatch.h \
    patternMatch/RisingWedgePatternMatch.h \
    patternScan/FallingWedgeScanner.h \
    patternMatch/FallingWedgePatternMatch.h
unix {
    target.path = /usr/local/lib
    INSTALLS += target
}
