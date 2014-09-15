#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricTriangleScanner.h"
#include "PatternShapeGenerator.h"
#include "DescendingTriangleScanner.h"
#include "MultiPatternScanner.h"
#include "PivotHighScanner.h"
#include "PivotLowScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( TriangleScanners_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    // TODO - The scanning seems to be working fine, but the actual pattern matches for a double-bottom don't appear to
    // be correct. In particular, the start dates for some of the matches occur below the RHS of the first dip in the
    // double bottom.
  //  verifyMatchList("TriangleScanners_VZ_SymetricTriangle: unfiltered matches",doubleBottoms,5);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: filtered matches",symetricTriangles,4);


    PatternMatchPtr triangleMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*triangleMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("TriangleScanners_SynthesizedPattern match",
            ptime(date(2013,7,29)),ptime(date(2013,10,7)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( TriangleScanners_GLD_Wedges )
{
    // The chart data from 2013 to 2014 contains a number of potential wedge matches.

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GLD_Weekly_2013_2014.csv");

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: filtered matches",symetricTriangles,0);


    DescendingTriangleScanner descTriangleScanner;
    PatternMatchListPtr descTriangles = descTriangleScanner.scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: descending triangles",descTriangles,0);

}



BOOST_AUTO_TEST_CASE( TriangleScanners_CELG_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CELG_20140501_20140814_Daily.csv");

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("TriangleScanners_CELG_SymetricTriangle: filtered matches",symetricTriangles,1);


    PatternMatchPtr wedgeMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("TriangleScanners_CELG_SymetricTriangle match",
            ptime(date(2014,7,3)),ptime(date(2014,8,13)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( TriangleScanners_GMCR_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GMCR_Daily_2014.csv");

    PatternMatchListPtr pivotHighs = PivotHighScanner().scanPatternMatches(chartData);
    PatternMatchListPtr pivotLows = PivotLowScanner().scanPatternMatches(chartData);

    genPivotHighInfo(pivotHighs);
    genPivotLowInfo(pivotLows);


    DescendingTriangleScanner descendingTriangleScanner;
    PatternMatchListPtr descTriangles = descendingTriangleScanner.scanPatternMatches(chartData);

    verifyMatchList("TriangleScanners_GMCR_Wedge: descending triangles",descTriangles,0);

    /*

      TODO - This test case previously matched the following. The FallingWedgeScanner
      is really matching DescendingTriangles and needs to be renamed to FallingTriangleScanner.
      With this in place, the pattern dated below should not match as a falling triange, but
      as a falling wedge (i.e., with both bottom and top trend-lines angled downward).
    verifyPatternMatch("TriangleScanners_GMCR_Wedge falling wedge match",
            ptime(date(2014,5,13)),ptime(date(2014,6,6)),1,fallingWedges,0);

    PatternMatchPtr wedgeMatch = fallingWedges->back();
    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);
    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();
*/


}

BOOST_AUTO_TEST_CASE( TriangleScanners_CMG_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CMG_Daily.csv");

    PatternScannerPtr symetricScanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = symetricScanner->scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_GMCR_Wedge: symetric triangles",symetricTriangles,4);

}


