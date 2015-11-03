////////////////////////////////////////////////////////////////////////
//
//  LArProperties_plugin
//
////////////////////////////////////////////////////////////////////////
// Framework includes

// C++ language includes
#include <iostream>

// LArSoft includes
#include "Utilities/LArPropertiesService.h"
//#include "SimpleTypesAndConstants/PhysicalConstants.h"

// ROOT includes
#include "TMath.h"

// Framework includes
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cetlib/exception.h"
//-----------------------------------------------
util::LArPropertiesService::LArPropertiesService(fhicl::ParameterSet const& pset, art::ActivityRegistry &reg)
{
  this->reconfigure(pset);
  reg.sPreBeginRun.watch(this, &LArPropertiesService::preBeginRun);
}

//----------------------------------------------
void util::LArPropertiesService::preBeginRun(const art::Run& run)
{
  fProp->Update(run.id().run());
}



//------------------------------------------------
/// \todo these values should eventually come from a database
void util::LArPropertiesService::reconfigure(fhicl::ParameterSet const& pset)
{
  fProp->Configure(pset);  
  return;
}

//------------------------------------------------
namespace util{
 
  DEFINE_ART_SERVICE_INTERFACE_IMPL(util::LArPropertiesService, util::ILArPropertiesService)

} // namespace util
