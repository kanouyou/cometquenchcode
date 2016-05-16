#ifndef IFdmDataflow_HH
#define IFdmDataflow_HH

#include <string>
#include <map>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include "IFdmThermalSolver.h"

namespace FDM
{ class IFdmDataflow; }

class FDM::IFdmDataflow
{
  public:
    /*! constructor */
    IFdmDataflow();

    /*!
       @brief constructor
       @param (filename) root file name
     */
    IFdmDataflow(const std::string& filename);

    ~IFdmDataflow();

    /*! set root file name */
    void SetFile(const std::string& name);

    /*! 
       @brief set subfolder's name
       @param (foldername) subdirectory name
     */
    void SetSubFolder(const std::string& foldername);

    /*!
        @brief fill class data into tree
        @param (sol)  solenoid name
        @param (name) tree's name
        @param (mag)  solenoid data container
     */
    void Fill(const std::string& sol, const std::string& name, FDM::IFdmThermalSolver* mag);

    /*! close file */
    void Close();

  private:
    TFile* fFile;
    std::map<std::string, TTree*> fTree;
    std::map<std::string, TDirectory*> fDir;
};

#endif
