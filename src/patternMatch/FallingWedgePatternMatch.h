#ifndef FALLINGWEDGEPATTERNMATCH_H
#define FALLINGWEDGEPATTERNMATCH_H

#include "WedgePatternMatch.h"

class FallingWedgePatternMatch : public WedgePatternMatch
{
public:
    FallingWedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo);

    virtual std::string matchType() const { return "Falling Wedge"; }
};

#endif // FALLINGWEDGEPATTERNMATCH_H
