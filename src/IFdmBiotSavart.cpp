#include <cmath>
#include "IFdmUnits.h"
#include "IFdmBiotSavart.h"

FDM::IFdmBiotSavart::IFdmBiotSavart()
    : fSolenoid(NULL), fSolMesh(NULL),
      fACdt(4.75*mm * 15.05*mm)
{
  fSolMesh = new int[3];

  fSolMesh[0] = 10;
  fSolMesh[1] = 80;
  fSolMesh[2] = 80;
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
    for (int j=0; j<fSolMesh[1]; j++) {  // PHI
      for (int k=0; k<fSolMesh[2]; k++) {  // Z
        // calculate current point at solenoid
        R   = fSolenoid[0] + i * dr + dr/2;
        PHI = 0.           + j * dp + dp/2;
        Z   = fSolenoid[2] + k * dz + dz/2;
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


