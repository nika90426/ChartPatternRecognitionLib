/*
 * XYCoord.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */


#include "XYCoord.h"

XYCoord::XYCoord(double x, double y)
: x_(x), y_(y)
{

}

double XYCoord::x() const
{
    return x_;
}

double XYCoord::y() const
{
    return y_;
}


