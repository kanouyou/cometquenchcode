#include <iostream>
#include "IFdmFieldHandle.h"
#include "IFdmConstruction.h"
#include "IFdmDataflow.h"

FDM::IFdmConstruction::IFdmConstruction()
    : fTime(NULL), fDump(0.128),
      fIndct(12.69),fCurrent(2700.),
      fThV(0.01), fVd(0.7),
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
    fCS0 = new FDM::IFdmThermalSolver(35,1,9);

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
    fCS1 = new FDM::IFdmThermalSolver(270, 1, 9);

  fCS1->SetLayer(9);
  fCS1->SetTurn(270);
  fCS1->SetRRR(300.);
  fCS1->SetField(5.4);
  fCS1->SetCurrent(fCurrent);
  fCS1->Initialize();

  // set quench spot
  fCS1->GetContainer(135,1,2)->SetQuench(true);

}

/*! construct magnetic field for each magnet */
void FDM::IFdmConstruction::ConstructField(const std::string& name, FDM::IFdmThermalSolver* mag)
{
  FDM::IFdmFieldHandle* hand = new FDM::IFdmFieldHandle();
  hand->SetCurrent(fCurrent);
  // CS0
  hand->ConstructSolenoid("CS0", 672.*mm, 823.65*mm,  857.88*mm, 1038.12*mm);
  hand->SetMesh("CS0", 9, 35);
  // CS1
  hand->ConstructSolenoid("CS1", 672.*mm, 823.65*mm, -595.25*mm,  795.25*mm);
  hand->SetMesh("CS1", 9, 270);

  hand->EvalField(name);

  for (int i=1; i<mag->GetMesh()[0]+1; i++) {
    for (int j=1; j<mag->GetMesh()[1]+1; j++) {
      for (int k=1; k<mag->GetMesh()[2]+1; k++) {
        mag->GetContainer(i,j,k)->SetField( hand->GetContainer(name, k-1, i-1)->GetField()[2] );
//        std::cout << k << " " << hand->GetContainer(name, k-1, i-1)->GetId()[0] << " "
//                  << i << " " << hand->GetContainer(name, k-1, i-1)->GetId()[1] << std::endl;
      }
    }
  }

}

double FDM::IFdmConstruction::CurrentDecay(double R, double t)
{
  double Rtot = fDump + R;
  double tau  = fIndct / Rtot;
  double I    = (fCurrent + fVd/Rtot) * exp(-t / tau) - fVd/Rtot;

  return I;
}

void FDM::IFdmConstruction::FieldDecay(FDM::IFdmThermalSolver* mag, double factor)
{
  double B;

  for (int i=1; i<mag->GetMesh()[0]+1; i++) {
    for (int j=1; j<mag->GetMesh()[1]+1; j++) {
      for (int k=1; k<mag->GetMesh()[2]+1; k++) {
        B = mag->GetContainer(i,j,k)->GetField() - mag->GetContainer(i,j,k)->GetField() * factor;
        mag->GetContainer(i,j,k)->SetField( B );
      }
    }
  }

} 

void FDM::IFdmConstruction::TimeLoop()
{
  double dt   = fTime[2];
  double time = fTime[0];
  int    cnt  = 0;

  double magres  = 0.;
  double qchtime = 0.;
  double I  = fCurrent;
  double dcyfactor = 1.;
  double dI;
  double bt1, bt2;

  std::cout << "Constructing magnet..." << std::endl;

  // initialized CS0 field
  ConstructField("CS0", fCS0);

  // initialized CS1 field
  ConstructField("CS1", fCS1);

  // set data file
  std::string name;
  FDM::IFdmDataflow* data = new FDM::IFdmDataflow("capturequench.root");
  data->SetSubFolder("CS0");
  data->SetSubFolder("CS1");

  std::cout << "Start Time loop..." << std::endl;

  // time loop
  while (time < fTime[1] || I < 0.) {

    time += dt;

    // calculate magnet resistance
    magres = 0.;
    magres += fCS0->GetResistance();
    magres += fCS1->GetResistance();

    // calculate current and field decay
    if (magres*I >= fThV) {
      // calculate time local after magnet quenched
      qchtime += dt;
      // calculate current decay
      dI = I - CurrentDecay(magres, qchtime);
      I  = CurrentDecay(magres, qchtime);
      dcyfactor = dI / I;
      // calculate field decay
      FieldDecay(fCS0, dcyfactor);
      FieldDecay(fCS1, dcyfactor);
    }

    // upgrade property
    fCS0->Upgrade(I);
    fCS1->Upgrade(I);

    // solve thermal conduct equation
    bt1 = fCS0->Solve(dt);
    bt2 = fCS1->Solve(dt);
    if (bt1 > bt2)
      dt = bt1;
    else
      dt = bt2;

    dt /= 3.;

    // connect magnets
    fCS0->GetContainer(0,1,9)->SetTemp( fCS1->GetContainer(270,1,1)->GetTemp() );
    fCS1->GetContainer(271,1,1)->SetTemp( fCS0->GetContainer(1,1,9)->GetTemp() );

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
              //<< std::setw(9) << magres
              << std::setw(9) << fCS1->GetContainer(45,1,2)->GetSource()
              << std::setw(9) << std::fixed << fCS1->GetContainer(45,1,2)->GetTemp()
              //<< std::setw(9) << fCS0->GetContainer(35,1,9)->GetTemp()
              << std::setw(9) << fCS0->GetContainer(1,1,9)->GetTemp()
              << std::setw(7) << fCS1->GetContainer(135,1,1)->GetField() << std::endl;

    if (cnt%50==0) {
      name = std::string(Form("%.2f sec", time));

      data->Fill("CS0", name, fCS0);
      data->Fill("CS1", name, fCS1);
    }

    cnt ++;
  }

  data->Close();
}
