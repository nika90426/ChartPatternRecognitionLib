/*
 * MathHelper.h
 *
 *  Created on: Jun 27, 2014
 *      Author: sroehling
 */

#ifndef MATHHELPER_H_
#define MATHHELPER_H_

class MathHelper {
public:
	MathHelper();

	static double maxDouble();
	static double minDouble();
	static double relativePercentVal(double comparisonVal, double baseVal);
	static double absRelPercentVal(double comparisonVal, double baseVal);
    static bool valuesClose(double val1, double val2, double tolerance);

	virtual ~MathHelper();
};

#endif /* MATHHELPER_H_ */
