#ifndef SUSY_HLT_DoublePhoton_H
#define SUSY_HLT_DoublePhoton_H

//event
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//DQM
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//Photons
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


class SUSY_HLT_DoublePhoton: public DQMEDAnalyzer{
    
public:
    SUSY_HLT_DoublePhoton(const edm::ParameterSet& ps);
    virtual ~SUSY_HLT_DoublePhoton();
    
protected:
    void dqmBeginRun(edm::Run const &, edm::EventSetup const &) override;
    void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
    void analyze(edm::Event const& e, edm::EventSetup const& eSetup);
    void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup) ;
    void endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
    void endRun(edm::Run const& run, edm::EventSetup const& eSetup);
    
private:
    //histos booking function
    void bookHistos(DQMStore::IBooker &);
    
    //variables from config file
    edm::EDGetTokenT<reco::PhotonCollection> thePhotonCollection_;
    edm::EDGetTokenT<edm::TriggerResults> triggerResults_;
    edm::EDGetTokenT<trigger::TriggerEvent> theTrigSummary_;
    
    
    HLTConfigProvider fHltConfig;
    
    std::string HLTProcess_;
    std::string triggerPath_;
    std::string triggerPathAuxiliary_;
    edm::InputTag triggerFilter_;
    
    // Histograms
    MonitorElement* h_triggerPhoPt;
    MonitorElement* h_triggerPhoEta;
    MonitorElement* h_triggerPhoPhi;
    MonitorElement* h_PhoTurnOn_num;
    MonitorElement* h_PhoTurnOn_den;
    MonitorElement* h_triggerDoublePhoMass;
};

#endif
