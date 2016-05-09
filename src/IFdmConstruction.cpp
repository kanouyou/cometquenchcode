#include <iostream>
#include "IFdmConstruction.h"

FDM::IFdmConstruction::IFdmConstruction()
    : fTime(NULL), fDump(0.128),
      fIndct(12.69),fCurrent(2700.),
      fThV(0.01),
      fCS0(NULL), fCS1(NULL)
{}

FDM::IFdmConstruction::~IFdmConstruction()
{
  if (fTime) delete[] fTime;
}

void FDM::IFdmConstruction::SetTime(double t0, double tf, double dt)
{
  if (!fTime) fTime = new double[3];

  fTime[0] = t0;
  fTime[1] = tf;
  fTime[2] = dt;
}

void FDM::IFdmConstruction::Construct()
{
  // ************************** //
  // CS0
  // ************************** //
  if (!fCS0)
    fCS0 = new IFdmThermalSolver(35,4,9);

  fCS0->SetLayer(9);
  fCS0->SetTurn(35);
  fCS0->SetRRR(100.);
  fCS0->SetField(5.0);
  fCS0->SetCurrent(fCurrent);
  fCS0->Initialize();

  // ************************** //
  // CS1
  // ************************** //
  if (!fCS1)
    fCS1 = new IFdmThermalSolver(270, 4, 9);

  fCS1->SetLayer(9);
  fCS1->SetTurn(270);
  fCS1->SetRRR(200.);
  fCS1->SetField(5.4);
  fCS1->SetCurrent(fCurrent);
  fCS1->Initialize();

  // set quench spot
  fCS1->GetContainer(45,1,2)->SetQuench(true);
}

void FDM::IFdmConstruction::TimeLoop()
{
  double dt   = fTime[2];
  double time = fTime[0];
  int    cnt  = 0;
  const double Vd = 0.7;

  double magres;
  double qchtime = 0.;
  double I  = fCurrent;
  double B0 = fCS0->GetContainer(45,2,2)->GetField();
  double B1 = fCS1->GetContainer(1,1,1)->GetField();
  double Bb[2] = {B0, B1};
  double B [2] = {B0, B1};

  // time loop
  while (time < fTime[1]) {

    time += dt;

    // calculate magnet resistance
    magres = 0.;
    magres += fCS0->GetResistance();
    magres += fCS1->GetResistance();

    // calculate current and field decay
    if (magres*I >= fThV) {
      qchtime += dt;
      I = ( fCurrent + Vd / (magres + fDump) ) * exp (-(magres + fDump) * qchtime / fIndct )
          - Vd / (magres + fDump);
      for (int i=0; i<sizeof(B)/sizeof(B[0]); i++) 
        B[i] = Bb[i] * exp(-(magres + fDump) * qchtime / fIndct );
    }

    // upgrade property
    fCS0->Upgrade(I, B[0]);
    fCS1->Upgrade(I, B[1]);

    // solve thermal conduct equation
    fCS0->Solve(dt);
    dt = fCS1->Solve(dt) / 2.;

    // connect magnets
    fCS0->GetContainer(0,1,9)->SetTemp( fCS1->GetContainer(270,1,9)->GetTemp() );
    fCS1->GetContainer(271,1,9)->SetTemp( fCS0->GetContainer(1,1,9)->GetTemp() );

    if (cnt%4==0)
/*    std::cout << " Time: " << time << " [sec] " << " step: " << dt / msec << " [msec]"
              << " current: " << I << " [A] "
              << " voltage: " << magres * I << " [V]"
              << " heat: " << fCS1->GetContainer(48,2,2)->GetSource()
              << " Temperature: " << fCS1->GetContainer(45,2,2)->GetTemp()
              << "   " << fCS1->GetContainer(50,3,9)->GetTemp() << std::endl;
*/
    std::cout << std::fixed   << std::setprecision(2) << time 
              << std::setw(7) << dt / msec
              << std::setw(9) << I
              << std::setw(9) << std::scientific << magres * I
              << std::setw(9) << fCS1->GetContainer(45,2,2)->GetSource()
              << std::setw(9) << std::fixed << fCS1->GetContainer(45,2,2)->GetTemp()
              << std::setw(9) << fCS0->GetContainer(1,1,9)->GetTemp()
              << std::setw(7) << fCS1->GetContainer(45,2,2)->GetField() << std::endl;

    cnt ++;
  }

  
}
