/*
 * PatternMatchVisitor.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVISITOR_H_
#define PATTERNMATCHVISITOR_H_

class SymetricWedgePatternMatch;
class VPatternMatch;
class DoubleBottomPatternMatch;
class CupPatternMatch;

class PatternMatchVisitor {
public:
	PatternMatchVisitor();

    virtual void visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &) {}
    virtual void visitVPatternMatch(VPatternMatch &) {}
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &) {}
    virtual void visitCupPatternMatch(CupPatternMatch &) {}

	virtual ~PatternMatchVisitor();
};

#endif /* PATTERNMATCHVISITOR_H_ */
