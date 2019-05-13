#ifndef __HARDTLIB_H
#define __HARDTLIB_H

/********************************************************************
Initialization, logging and other basic functions.
(implemented in hardt.cpp)
********************************************************************/

#include "hardt.h"
#include "hexceptions.h"
#include "hsoundcard.h"
#include "hwav.h"
#include "hmetrics.h"
#include "hobject.h"
#include "hgenerator.h"

/********************************************************************
Abstract base classes with no, or little implementation
********************************************************************/

#include "hwriter.cpp"
#include "hreader.cpp"
#include "hprocessor.cpp"

/********************************************************************
Include api function declarations
********************************************************************/

#include "hsoundcardreader.cpp"
#include "hfilewriter.cpp"
#include "hfilereader.cpp"
#include "hwavwriter.cpp"
#include "hwavreader.cpp"
#include "hnetworkwriter.cpp"
#include "hnetworkreader.cpp"
#include "hnetworkprocessor.cpp"
#include "hsinegenerator.cpp"
#include "hcosinegenerator.cpp"

#endif