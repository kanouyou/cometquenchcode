#ifndef IFdmDataflow_HH
#define IFdmDataflow_HH

#include "IThermalContainer.h"

namespace FDM
{ class IFdmDataflow; }

class FDM::IFdmDataflow
{
  public:
    IFdmDataflow();

    IFdmDataflow(IThermalContainer* con);

    ~IFdmDataflow();

    void Save(IThermalContainer* con); 

    void SetMesh(const int z, const int p, const int r);

  private:
    IThermalContainer* fCollect;
    int* fMesh;
};

#endif
