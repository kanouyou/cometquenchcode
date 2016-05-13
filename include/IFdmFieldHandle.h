#ifndef IFdmFieldHandle_HH
#define IFdmFieldHandle_HH

#include <map>
#include <string>
#include <vector>
#include "IFdmField.h"

namespace Magnet
{ class IMagnetContainer; }

class Magnet::IMagnetContainer
{
  public:
    /*! constructor */
    IMagnetContainer() : fName(""), fSolenoid(NULL), fMesh(NULL) {}

    /*! deconstructor */
    ~IMagnetContainer() {
      if (fSolenoid)  delete[] fSolenoid;
      if (fMesh)      delete[] fMesh;
    }

    /*! save solenoid name */
    void SetName(const std::string& name) { fName = name; }

    /*! save solenoid dimension */
    void SetDimension(double r0, double r1, double z0, double z1) {
      if (!fSolenoid)  fSolenoid = new double[4];
      fSolenoid[0] = r0;
      fSolenoid[1] = r1;
      fSolenoid[2] = z0;
      fSolenoid[3] = z1;
    }

    /*! save solenoid mesh */
    void SetMesh(int mr, int mz) {
      if (!fMesh)  fMesh = new int[2];
      fMesh[0] = mr;
      fMesh[1] = mz;
    }

    /*! return solenoid name */
    std::string GetName() const { return fName; }

    /*! return solenoid dimension */
    double* GetDimension() { return fSolenoid; }

    /*! return solenoid mesh */
    int* GetMesh() { return fMesh; }

  private:
    std::string fName;
    double* fSolenoid;
    int*    fMesh;
};

/* ************************************** */

namespace FDM
{ class IFdmFieldHandle; }

class FDM::IFdmFieldHandle
{
  public:
    /*! constructor */
    IFdmFieldHandle();

    /*! deconstructor */
    ~IFdmFieldHandle();

    /*! set flowed current */
    void SetCurrent(double I) { fCurrent = I; }

    /*! construct solenoid dimension */
    void ConstructSolenoid(const std::string& name, double r0, double r1,
                                                    double z0, double z1);

    /*! set the mesh for this solenoid */ 
    void SetMesh(const std::string& name, const int mr, const int mz);

    /*! calculate field map */
    void EvalField(const std::string& name);

    /*! plot graph */
    void Plot(const std::string& name);

    /*! return field container at this point */
    Field::IFieldContainer* GetContainer(const std::string& name, int i, int j);

    /*! return the field container vector */
    std::vector<Field::IFieldContainer*> GetContainerVector() { return fFieldCollect; }

  private:
    int    fCount;
    double fCurrent;
    std::map<int, Magnet::IMagnetContainer*> fHC;
    std::vector<Field::IFieldContainer*> fFieldCollect;
};

#endif
