#ifndef IMyPostprocess_HH
#define IMyPostprocess_HH

#include <string>
#include <TFile.h>
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

  private:
    TFile* fOutput;
};

#endif
