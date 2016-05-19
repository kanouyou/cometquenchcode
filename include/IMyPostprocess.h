#ifndef IMyPostprocess_HH
#define IMyPostprocess_HH

#include <string>
#include <TFile.h>
#include <TH2F.h>
#include "IFdmPostprocess.h"

class IMyPostprocess : public FDM::IFdmPostprocess
{
  public:
    /*! @brief  constructor */
    IMyPostprocess();

    /*! @brief  deconstructor */
    virtual ~IMyPostprocess();

    /*! @brief  set output file's name */
    virtual void SetFile(const std::string& filename);

    /*! @brief  plot time vs temperature graph */
    virtual void PlotTimeTemp();

    /*! @brief  return the field plot at this time */
    virtual TH2F* GetFieldHist(const std::string& name);

    /*! @brief  plot 2D field */
    virtual void PlotField2D();

    /*! @brief  return 2d temperature distribution hist */
    virtual TH2F* GetTempHist(const std::string& name);

    /*! @brief  plot 2d temp */
    virtual void PlotTemp2D();

    /* @brief   return 2d heat distribution hist */
    virtual TH2F* GetHist(const std::string& name);

    /* @brief   plot 2d heat distribution */
    virtual void  PlotHeat2D();

  private:
    TFile* fOutput;
};

#endif
