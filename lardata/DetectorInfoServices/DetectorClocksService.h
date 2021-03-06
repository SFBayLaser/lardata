////////////////////////////////////////////////////////////////////////
// DetectorClocksService.h
//
// Pure virtual service interface for DetectorClocks functions
//
//  jpaley@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#ifndef DETECTORCLOCKSSERVICE_H
#define DETECTORCLOCKSSERVICE_H

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "lardataalg/DetectorInfo/DetectorClocks.h"
#include "larcore/CoreUtils/ServiceUtil.h"
#include "larcore/CoreUtils/EnsureOnlyOneSchedule.h"

///General LArSoft Utilities
namespace detinfo{
  class DetectorClocksService: private lar::EnsureOnlyOneSchedule<DetectorClocksService> {

    public:
    typedef detinfo::DetectorClocks provider_type;

    public:
      virtual ~DetectorClocksService() = default;

      virtual void   reconfigure(fhicl::ParameterSet const& pset) = 0;
      virtual const  detinfo::DetectorClocks* provider() const = 0;

    }; // class DetectorClocksService
} //namespace detinfo
DECLARE_ART_SERVICE_INTERFACE(detinfo::DetectorClocksService, SHARED)
#endif // DETECTORCLOCKSSERVICE_H
