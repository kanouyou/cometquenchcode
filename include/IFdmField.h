#ifndef IFdmField_HH
#define IFdmField_HH

#include <vector>
#include <cmath>
#include "IFdmBiotSavart.h"

/* ********************************************* */

namespace Field
{ class IFieldContainer; }

class Field::IFieldContainer
{
  public:
    IFieldContainer() : fPos(NULL), fField(NULL), fId(NULL) {}

    ~IFieldContainer() {
      if (fPos) delete[] fPos;
    }

    void SetId(double ir, double iz) {
      if (!fId)  fId = new int[2];
      fId[0] = ir;
      fId[1] = iz;
    }

    void SetPosition(double r, double z) {
      if (!fPos)  fPos = new double[2];
      fPos[0] = r;
      fPos[1] = z;
    }

    void SetField(double Br, double Bz) {
      if (!fField)  fField = new double[3];
      fField[0] = Br;
      fField[1] = Bz;
      fField[2] = sqrt(pow(Br,2) + pow(Bz,2));
    }

    void SetPotential(double A) { fA = A; }

    double* GetPosition() { return fPos; }

    double* GetField() { return fField; }

    double  GetPotential() { return fA; }

    int*    GetId() { return fId; }

  private:
    double* fPos;
    double* fField;
    double  fA;
    int*    fId;
};

/* ********************************************* */

namespace FDM
{ class IFdmField; }

class FDM::IFdmField : public FDM::IFdmBiotSavart
{
  public:
    /*! Constructor */
    IFdmField();

    /*! Deconstructor */
    ~IFdmField();

    /*! Set calculated region mesh */
    void SetMapMesh(const int mr, const int mz);

    /*! Set calculated region */
    void SetMap(double r0, double r1, double z0, double z1);

    /*! calculate field */
    void EvalField();

    /*! calculate solenoid self inductance */
    double GetSelfInductance();

    /*! return the field container at this point (i, j) */
    Field::IFieldContainer* GetContainer(const int i, const int j);

    /*! return the whole container vector */
    std::vector<Field::IFieldContainer*> GetCollection() { return fHC; }

    /*! plot field map */
    void Plot();

  private:
    int*    fMapMesh;
    double* fMap;
    std::vector<Field::IFieldContainer*> fHC;
};

#endif
