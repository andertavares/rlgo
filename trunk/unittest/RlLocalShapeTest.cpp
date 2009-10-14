//----------------------------------------------------------------------------
/** @file RlLocalShapeTrackerTest.cpp
    Unit tests for RlLocalShapeTracker
*/
//----------------------------------------------------------------------------

#include "SgSystem.h"

#include <boost/test/auto_unit_test.hpp>
#include "RlLocalShapeTracker.h"

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "RlLocalShape.h"
#include "RlLocalShapeFeatures.h"
#include "RlLocalShapeTracker.h"
#include "RlUtils.h"
#include "RlTestUtil.h"

using namespace std;
using namespace SgPointUtil;
using namespace boost::test_tools;

//----------------------------------------------------------------------------

namespace {

// All of the following tests are based on the shapes in this position:

// . . . . .
// . . O . .
// . O X O .
// . X . X X
// . . . . .

BOOST_AUTO_TEST_CASE(RlLocalMoveTest)
{
    GoBoard bd(5);
    RlLocalShapeFeatures shapes(bd, 2, 2);
    shapes.EnsureInitialised();

    // Identify features
    int f1 = shapes.GetFeature("01-01-..-.X");
    int f2 = shapes.GetFeature("02-02-X.-OX");
    int f3 = shapes.GetFeature("03-02-.X-XO");
    int f4 = shapes.GetFeature("04-01-..-XX");
    int f5 = shapes.GetFeature("04-02-XX-O.");
    BOOST_CHECK(f1 != -1);
    BOOST_CHECK(f2 != -1);
    BOOST_CHECK(f3 != -1);
    BOOST_CHECK(f4 != -1);
    BOOST_CHECK(f5 != -1);

    BOOST_CHECK(shapes.LocalMove(f1, 0, 0, 1) 
        == shapes.GetFeature("01-01-X.-.X"));
    BOOST_CHECK(shapes.LocalMove(f1, 1, 0, 2) 
        == shapes.GetFeature("01-01-.O-.X"));
    BOOST_CHECK(shapes.LocalMove(f1, 1, 1, 0) 
        == shapes.GetFeature("01-01-..-.."));
    BOOST_CHECK(shapes.LocalMove(f1, 1, 1, 1) == -1); 
    BOOST_CHECK(shapes.LocalMove(f2, 1, 0, 1) 
        == shapes.GetFeature("02-02-XX-OX"));
    BOOST_CHECK(shapes.LocalMove(f3, 0, 0, 2) 
        == shapes.GetFeature("03-02-OX-XO"));
    BOOST_CHECK(shapes.LocalMove(f4, 1, 0, 2) 
        == shapes.GetFeature("04-01-.O-XX"));
    BOOST_CHECK(shapes.LocalMove(f5, 1, 1, 2) 
        == shapes.GetFeature("04-02-XX-OO"));
}

BOOST_AUTO_TEST_CASE(RlSuccessorTest)
{
    GoBoard bd(5);
    RlLocalShapeFeatures shapes(bd, 2, 2);
    shapes.EnsureInitialised();
    RlLocalShapeTracker tracker(bd, &shapes);
    tracker.Initialise();

    // Identify features
    int f1 = shapes.GetFeature("01-01-..-.X");
    int f2 = shapes.GetFeature("02-02-X.-OX");
    int f3 = shapes.GetFeature("03-02-.X-XO");
    int f4 = shapes.GetFeature("04-01-..-XX");
    int f5 = shapes.GetFeature("04-02-XX-O.");
    BOOST_CHECK(f1 != -1);
    BOOST_CHECK(f2 != -1);
    BOOST_CHECK(f3 != -1);
    BOOST_CHECK(f4 != -1);
    BOOST_CHECK(f5 != -1);

    BOOST_CHECK(tracker.GetSuccessor(f1, 0, 0, 1) 
        == shapes.GetFeature("01-01-X.-.X"));
    BOOST_CHECK(tracker.GetSuccessor(f1, 1, 0, 2) 
        == shapes.GetFeature("01-01-.O-.X"));
    BOOST_CHECK(tracker.GetSuccessor(f1, 1, 1, 0) 
        == shapes.GetFeature("01-01-..-.."));
    // currently causes assertion
    // BOOST_CHECK(tracker.GetSuccessor(f1, 1, 1, 1) == -1); 
    BOOST_CHECK(tracker.GetSuccessor(f2, 1, 0, 1) 
        == shapes.GetFeature("02-02-XX-OX"));
    BOOST_CHECK(tracker.GetSuccessor(f3, 0, 0, 2) 
        == shapes.GetFeature("03-02-OX-XO"));
    BOOST_CHECK(tracker.GetSuccessor(f4, 1, 0, 2) 
        == shapes.GetFeature("04-01-.O-XX"));
    BOOST_CHECK(tracker.GetSuccessor(f5, 1, 1, 2) 
        == shapes.GetFeature("04-02-XX-OO"));
}

BOOST_AUTO_TEST_CASE(RlLocalShapeTrackerTest)
{
    GoBoard bd(5);
    RlLocalShapeFeatures shapes(bd, 2, 2);
    shapes.EnsureInitialised();
    RlLocalShapeTracker tracker(bd, &shapes);
    tracker.Initialise();

    // Identify features
    int f1 = shapes.GetFeature("01-01-..-.X");
    int f2 = shapes.GetFeature("02-02-X.-OX");
    int f3 = shapes.GetFeature("03-02-.X-XO");
    int f4 = shapes.GetFeature("04-01-..-XX");
    int f5 = shapes.GetFeature("04-02-XX-O.");
    BOOST_CHECK(f1 != -1);
    BOOST_CHECK(f2 != -1);
    BOOST_CHECK(f3 != -1);
    BOOST_CHECK(f4 != -1);
    BOOST_CHECK(f5 != -1);
    
    // Play moves
    Reset(tracker, shapes);
    Play(Pt(3, 3), SG_BLACK, bd, tracker, shapes);
    Play(Pt(2, 3), SG_WHITE, bd, tracker, shapes);
    Play(Pt(2, 2), SG_BLACK, bd, tracker, shapes);
    Play(Pt(3, 4), SG_WHITE, bd, tracker, shapes);
    Play(Pt(4, 2), SG_BLACK, bd, tracker, shapes);
    Play(Pt(4, 3), SG_WHITE, bd, tracker, shapes);
    Play(Pt(5, 2), SG_BLACK, bd, tracker, shapes);

    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f1), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f2), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f3), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f4), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f5), 1);
    BOOST_CHECK_EQUAL(tracker.Active().GetTotalActive(), 16);

    // Undo moves again
    for (int i = 0; i < 7; ++i)
        Undo(bd, tracker, shapes);

    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f1), 0);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f2), 0);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f3), 0);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f4), 0);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f5), 0);
    BOOST_CHECK_EQUAL(tracker.Active().GetTotalActive(), 16);

    // Start game and test Reset() from current position
    bd.Init(5);
    bd.Play(Pt(3, 3), SG_BLACK);
    bd.Play(Pt(2, 3), SG_WHITE);
    bd.Play(Pt(2, 2), SG_BLACK);
    bd.Play(Pt(3, 4), SG_WHITE);
    bd.Play(Pt(4, 2), SG_BLACK);
    bd.Play(Pt(4, 3), SG_WHITE);
    bd.Play(Pt(5, 2), SG_BLACK);
    Reset(tracker, shapes);

    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f1), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f2), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f3), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f4), 1);
    BOOST_CHECK_EQUAL(CountOccurrences(tracker, f5), 1);
    BOOST_CHECK_EQUAL(tracker.Active().GetTotalActive(), 16);
}

} // namespace

//----------------------------------------------------------------------------
