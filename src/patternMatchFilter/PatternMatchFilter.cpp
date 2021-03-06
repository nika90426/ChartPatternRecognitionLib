/*
 * PatternMatchFilter.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <PatternMatchFilter.h>
#include <algorithm>

namespace patternMatchFilter
{

// The STL sort and unique functions take values instead of references as arguments.
// Therefore, when passing a derived functor, they will slice the derived part. These
// proxy objects allow the objects to still be passed by value, but pass the derived
// sort and unique functors as arguments to the common sort and filter functions.
class SortPatternMatchProxy
{
private:
	const PatternMatchSortFunctor &cmp_;
public:
    SortPatternMatchProxy(const PatternMatchSortFunctor &cmp) : cmp_(cmp) {}
    bool operator()(const PatternMatchPtr &match1, const PatternMatchPtr &match2) const { return cmp_(match1, match2); }
};

class UniquePatternMatchProxy
{
private:
	const PatternMatchUniqueFunctor &func_;
public:
	UniquePatternMatchProxy(const PatternMatchUniqueFunctor &func) : func_(func) {}
    bool operator()(const PatternMatchPtr &match1, const PatternMatchPtr &match2) const { return func_(match1, match2); }
};


class FindPatternMatchProxy
{
private:
	const PatternMatchFindPredicate &pred_;
public:
	FindPatternMatchProxy(const PatternMatchFindPredicate &pred) : pred_(pred) {}
    bool operator()(const PatternMatchPtr &match) const { return pred_(match); }
};


PatternMatchListPtr sortPatternMatches(const PatternMatchListPtr &unsortedMatches,
		const PatternMatchSortFunctor &sortFunc)
{
	// Copy the original list
	PatternMatchListPtr sortedList(new PatternMatchList);
	sortedList->insert(sortedList->end(),unsortedMatches->begin(), unsortedMatches->end());

	sortedList->sort(SortPatternMatchProxy(sortFunc));

	return sortedList;

}

PatternMatchListPtr uniquePatternMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchUniqueFunctor &uniqueFunc)
{
	// Copy the original list
	PatternMatchListPtr filteredList(new PatternMatchList);
	filteredList->insert(filteredList->end(),unfilteredMatches->begin(), unfilteredMatches->end());

	filteredList->unique(UniquePatternMatchProxy(uniqueFunc));

	return filteredList;
}

PatternMatchListPtr sortThenFilterUniqueMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchSortFunctor &sortFunc, const PatternMatchUniqueFunctor &uniqueFunc)
{
	PatternMatchListPtr filteredList = sortPatternMatches(unfilteredMatches,sortFunc);

	return uniquePatternMatches(filteredList,uniqueFunc);

}

PatternMatchListPtr filterUniqueStartEndTime(const PatternMatchListPtr &unfilteredMatches)
{
	return sortThenFilterUniqueMatches(unfilteredMatches,
			SortPatternMatchByStartAndEndDate(),SameStartAndEndDate());
}

PatternMatchListPtr filterUniqueAndLongestHighestHigh(const PatternMatchListPtr &unfilteredMatches)
{
	return sortThenFilterUniqueMatches(unfilteredMatches,
			SortPatternMatchByHighestHighTimeThenLength(),SameHighestHighTime());
}

PatternMatchListPtr filterUniqueAndLongestLowestLow(const PatternMatchListPtr &unfilteredMatches)
{
	return sortThenFilterUniqueMatches(unfilteredMatches,
			SortPatternMatchByLowestLowTimeThenLength(),SameLowestLowTime());
}

PatternMatchListPtr filterUniqueLongestPatternSameEndDate(const PatternMatchListPtr &unfilteredMatches)
{
    PatternMatchListPtr  sortedMatches = patternMatchFilter::sortPatternMatches(
                unfilteredMatches,SortPatternMatchByEndTimeThenLength());

    // Eliminate duplicate patterns with the same end date (confirmation date), but differenting start dates.
    // We only want to return the pattern starting earliest, essentially the one with same end date, but starting
    // the earliest.
    PatternMatchListPtr  uniqueMatches = patternMatchFilter::uniquePatternMatches(sortedMatches,SameEndTime());

    // Sort the final results by start date, then end date.
    PatternMatchListPtr  sortedUniqueMatches = patternMatchFilter::sortPatternMatches(
                uniqueMatches,SortPatternMatchByStartAndEndDate());


    return uniqueMatches;
}




PatternMatchList::iterator findFirstPatternMatch(const PatternMatchListPtr &patternMatches,
		const PatternMatchFindPredicate &findPred)
{
	return std::find_if(patternMatches->begin(),patternMatches->end(),FindPatternMatchProxy(findPred));
}

} // namespace
