#include <cmath>
#include <iostream>
#include "IFdmUnits.h"
#include "IFdmBiotSavart.h"

using std::map;

FDM::IFdmBiotSavart::IFdmBiotSavart()
    : fSolenoid(NULL), fSolMesh(NULL),
      fACdt(4.75*mm * 15.05*mm)
{
  fSolMesh = new int[3];

  fSolMesh[0] = 10;
  fSolMesh[1] = 80;
  fSolMesh[2] = 80;

  int n = 4;

  switch (n) {
    case 1:
      fGaus.insert( map<double, double>::value_type(0., 2.) );
      break;
    case 2:
      fGaus.insert( map<double, double>::value_type(-1./sqrt(3.), 1.) );
      fGaus.insert( map<double, double>::value_type( 1./sqrt(3.), 1.) );
      break;
    case 3:
      fGaus.insert( map<double, double>::value_type(-sqrt(3./5.), 5./9.) );
      fGaus.insert( map<double, double>::value_type(          0., 8./9.) );
      fGaus.insert( map<double, double>::value_type( sqrt(3./5.), 5./9.) );
      break;
    case 4:
      fGaus.insert( map<double, double>::value_type(-sqrt(3./7. + 2.*sqrt(6./5.)/7.), (18.-sqrt(30.))/36.) );
      fGaus.insert( map<double, double>::value_type(-sqrt(3./7. - 2.*sqrt(6./5.)/7.), (18.+sqrt(30.))/36.) );
      fGaus.insert( map<double, double>::value_type( sqrt(3./7. - 2.*sqrt(6./5.)/7.), (18.+sqrt(30.))/36.) );
      fGaus.insert( map<double, double>::value_type( sqrt(3./7. + 2.*sqrt(6./5.)/7.), (18.-sqrt(30.))/36.) );
      break;
    default:
      break;
  }
}

FDM::IFdmBiotSavart::~IFdmBiotSavart()
{
  if (fSolenoid)  delete[] fSolenoid;
  if (fSolMesh)   delete[] fSolMesh;
}

void FDM::IFdmBiotSavart::SetSolenoid(double r0, double r1, double z1, double z2)
{
  if (!fSolenoid)  fSolenoid = new double[4];

  fSolenoid[0] = r0;
  fSolenoid[1] = r1;
  fSolenoid[2] = z1;
  fSolenoid[3] = z2;
}

void FDM::IFdmBiotSavart::SetSolenoidMesh(const int mr, const int mp, const int mz)
{
  if (!fSolMesh)  fSolMesh = new int[3];

  fSolMesh[0] = mr;
  fSolMesh[1] = mp;
  fSolMesh[2] = mz;
}

double FDM::IFdmBiotSavart::EvalPotential(double pr, double pz)
{
  const double dr = (fSolenoid[1] - fSolenoid[0]) / fSolMesh[0];
  const double dz = (fSolenoid[3] - fSolenoid[2]) / fSolMesh[2];
  const double dp = (2 * M_PI - 0.) / fSolMesh[1];

  double rPQ  = 0.;
  double Aphi = 0.;
  double R, Z, PHI, A;

  for (int i=0; i<fSolMesh[0]; i++) {  // R
    // calculate current point at solenoid
    R = fSolenoid[0] + i * dr + dr/2;
    for (int j=0; j<fSolMesh[1]; j++) {  // PHI
      // calculate current point at solenoid
      PHI = 0. + j * dp + dp/2;
      for (int k=0; k<fSolMesh[2]; k++) {  // Z
        // calculate current point at solenoid
        Z = fSolenoid[2] + k * dz + dz/2;

        // calculate rPQ
        rPQ = sqrt( pow(Z-pz,2) + pow(R,2) + pow(pr,2) - 2*pr*R*cos(PHI) );

        // integration
        if (rPQ==0.)
          Aphi += 0.;
        else {
          A = (mu0 * fCurrent / 4 / M_PI) * (R * cos(PHI) * dr * dz * dp / rPQ);
          Aphi += A;
        }

      }
    }
  }

  return Aphi;
}

double FDM::IFdmBiotSavart::EvalPotential2(double pr, double pz)
{
  double Aphi = 0.;

  const double r0 = fSolenoid[0];
  const double r1 = fSolenoid[1];
  const double z0 = fSolenoid[2];
  const double z1 = fSolenoid[3];

  double R, Z, PHI, rPQ, gw, A;

  for (map<double, double>::iterator it=fGaus.begin(); it!=fGaus.end(); it++) {
    for (map<double, double>::iterator jt=fGaus.begin(); jt!=fGaus.end(); jt++) {
      for (map<double, double>::iterator kt=fGaus.begin(); kt!=fGaus.end(); kt++) {
        R   = r0 + (it->first + 1) * (r1-r0) / 2;
        Z   = z0 + (jt->first + 1) * (z1-z0) / 2;
        PHI = 0. + (kt->first + 1) * 2*M_PI / 2;

        rPQ = sqrt( pow(Z-pz,2) + pow(R,2) + pow(pr,2) - 2*pr*R*cos(PHI) ); 
        gw  = R * cos(PHI) / rPQ;
        A   = it->second * jt->second * kt->second * gw;
        Aphi += A;
        //std::cout << it->first << " " << jt->first << " " << kt->first << " " << std::endl;
      }
    }
  }
  Aphi = Aphi * (mu0 * fCurrent / 4 / M_PI) * (r1-r0) * (z1-z0) * M_PI / 2. / 2.;

  return Aphi;
}
