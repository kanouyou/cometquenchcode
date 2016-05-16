#ifndef IFdmPostprocess_HH
#define IFdmPostprocess_HH

#include <vector>
#include <string>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TGraph.h>
#include "IThermalContainer.h"

namespace FDM
{ class IFdmPostprocess; }

class FDM::IFdmPostprocess
{
  public:
    /*! @brief  costructor */
    IFdmPostprocess(const std::string& name="capturequench.root");

    /*! @brief  deconstructor */
    ~IFdmPostprocess();

    /*! @fn     void SetDirectory(const std::string& name)
        @brief  set subdirectory and save time to double
     */
    void SetDirectory(const std::string& name);

    /*! @brief  get a vector contain tree's key name
     */
    std::vector<std::string> GetTreeName();

    /*! @brief   get the mesh of this magnet 
        /example 
     */
    int* GetMesh();

    /*! @brief   return temperature at (t, z, phi, r)
        @return  temperature
     */
    double GetTemperature(const std::string& name, const int z, const int p, const int r);

    /*! @brief   plot temperature vs time
     */
    TGraph* GetTempGraph(const int z, const int p, const int r);

    /*! @brief   return TFile */
    TFile*  GetFile() { return fFile; }

  protected:
    std::vector<double> fTime;
    std::string fDirName;

  private:
    TFile* fFile;
};

#endif
