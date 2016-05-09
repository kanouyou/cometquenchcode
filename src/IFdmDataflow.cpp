#include <iostream>
#include "IFdmDataflow.h"

FDM::IFdmDataflow::IFdmDataflow()
    : fCollect(NULL), fMesh(NULL)
{}

FDM::IFdmDataflow::IFdmDataflow(IThermalContainer* con)
    : fCollect(con), fMesh(NULL)
{}

FDM::IFdmDataflow::~IFdmDataflow()
{
  if (fCollect)  delete fCollect;
  if (fMesh)     delete fMesh;
}

void FDM::IFdmDataflow::Save(IThermalContainer* con)
{
  fCollect = con;
}

void FDM::IFdmDataflow::SetMesh(const int z, const int p, const int r)
{
  if (!fMesh) fMesh = new int[3];

  fMesh[0] = z;
  fMesh[1] = p;
  fMesh[2] = r;
}


