#ifndef IFdmParameters_HH
#define IFdmParameters_HH

#include <vector>
#include "IFdmUnits.h"

//static std::vector<double> fMagnetRes;

static const double LPHI  = 700. * mm;
static const double WTAPE = 0.8  * mm;
static const double LZCDT = 4.73 * mm;
static const double LRCDT = 15.0 * mm;

static const double RAL = 7.3 / 9.3;
static const double RCU = 1.0 / 9.3;
static const double RSC = 1.0 / 9.3;

static const double RHOCDT = 4000.;

static const double ACDTT = (LZCDT + WTAPE) * (LRCDT * WTAPE);    // total conductor area
static const double ACDT  = LZCDT * LRCDT;                        // conductor area without tape area
static const double ATAPE = ACDTT - ACDT;
static const double AAL   = RAL * ACDT;
static const double ACU   = RCU * ACDT;
static const double ASC   = RSC * ACDT;

//
static const double DUMPRES = 0.128 * Ohm;
static const double IDT = 12.69;

#endif
