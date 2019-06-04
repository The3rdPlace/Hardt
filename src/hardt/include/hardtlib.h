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

/********************************************************************
Abstract base classes with no, or little implementation
********************************************************************/

#include "hwriter.cpp"
#include "hcustomwriter.h"
#include "hreader.cpp"
#include "hprocessor.cpp"
#include "hgenerator.cpp"
#include "hfilter.cpp"

/********************************************************************
Include api function declarations
********************************************************************/

#include "hchunkwriter.cpp"
#include "hconverter.h"
#include "hnullwriter.cpp"
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
#include "hvfo.cpp"
#include "hstreamprocessor.cpp"
#include "hsoundcardwriter.cpp"
#include "hwindow.cpp"
#include "hfft.cpp"
#include "hrectangularwindow.cpp"
#include "hhammingwindow.cpp"
#include "hhahnwindow.cpp"

#endif