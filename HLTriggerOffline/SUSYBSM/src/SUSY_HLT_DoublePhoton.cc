#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "HLTriggerOffline/SUSYBSM/interface/SUSY_HLT_DoublePhoton.h"

SUSY_HLT_DoublePhoton::SUSY_HLT_DoublePhoton(const edm::ParameterSet& ps)
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "Constructor SUSY_HLT_DoublePhoton::SUSY_HLT_DoublePhoton " << std::endl;
    // Get parameters from configuration file
    theTrigSummary_ = consumes<trigger::TriggerEvent>(ps.getParameter<edm::InputTag>("trigSummary"));
    thePhotonCollection_ = consumes<reco::PhotonCollection>(ps.getParameter<edm::InputTag>("PhotonCollection"));
    triggerResults_ = consumes<edm::TriggerResults>(ps.getParameter<edm::InputTag>("TriggerResults"));
    HLTProcess_ = ps.getParameter<std::string>("HLTProcess");
    triggerPath_ = ps.getParameter<std::string>("TriggerPath");
    triggerPathAuxiliary_ = ps.getParameter<std::string>("TriggerPathAuxiliary");
    triggerFilter_ = ps.getParameter<edm::InputTag>("TriggerFilter");
}

SUSY_HLT_DoublePhoton::~SUSY_HLT_DoublePhoton()
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "Destructor SUSY_HLT_DoublePhoton::~SUSY_HLT_DoublePhoton " << std::endl;
}

void SUSY_HLT_DoublePhoton::dqmBeginRun(edm::Run const &run, edm::EventSetup const &e)
{
    
    bool changed;
    
    if (!fHltConfig.init(run, e, HLTProcess_, changed)) {
        edm::LogError("SUSY_HLT_DoublePhoton") << "Initialization of HLTConfigProvider failed!!";
        return;
    }
    
    bool pathFound = false;
    const std::vector<std::string> allTrigNames = fHltConfig.triggerNames();
    for(size_t j = 0; j <allTrigNames.size(); ++j) {
        if(allTrigNames[j].find(triggerPath_) != std::string::npos) {
            pathFound = true;
        }
    }
    
    if(!pathFound) {
        LogDebug("SUSY_HLT_DoublePhoton") << "Path not found" << "\n";
        return;
    }
    //std::vector<std::string> filtertags = fHltConfig.moduleLabels( triggerPath_ );
    //triggerFilter_ = edm::InputTag(filtertags[filtertags.size()-1],"",fHltConfig.processName());
    //triggerFilter_ = edm::InputTag("hltPFMET120Mu5L3PreFiltered", "", fHltConfig.processName());
    
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "susy_HLT_DoublePhoton::beginRun" << std::endl;
}

void SUSY_HLT_DoublePhoton::bookHistograms(DQMStore::IBooker & ibooker_, edm::Run const &, edm::EventSetup const &)
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "SUSY_HLT_DoublePhoton::bookHistograms" << std::endl;
    //book at beginRun
    bookHistos(ibooker_);
}

void SUSY_HLT_DoublePhoton::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg,
                                                        edm::EventSetup const& context)
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "SUSY_HLT_DoublePhoton::beginLuminosityBlock" << std::endl;
}



void SUSY_HLT_DoublePhoton::analyze(edm::Event const& e, edm::EventSetup const& eSetup){
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "SUSY_HLT_DoublePhoton::analyze" << std::endl;
    
    //-------------------------------
    //--- Photon
    //-------------------------------
    edm::Handle<reco::PhotonCollection> PhotonCollection;
    e.getByToken (thePhotonCollection_, PhotonCollection);
    if ( !PhotonCollection.isValid() ){
        edm::LogError ("SUSY_HLT_DoublePhoton") << "invalid collection: Photons " << "\n";
        return;
    }
    
    
    //-------------------------------
    //--- Trigger
    //-------------------------------
    edm::Handle<edm::TriggerResults> hltresults;
    e.getByToken(triggerResults_,hltresults);
    if(!hltresults.isValid()){
        edm::LogError ("SUSY_HLT_DoublePhoton") << "invalid collection: TriggerResults" << "\n";
        return;
    }
    edm::Handle<trigger::TriggerEvent> triggerSummary;
    e.getByToken(theTrigSummary_, triggerSummary);
    if(!triggerSummary.isValid()) {
        edm::LogError ("SUSY_HLT_DoublePhoton") << "invalid collection: TriggerSummary" << "\n";
        return;
    }
    
    
    //get online objects
    std::vector<float> ptPhoton, etaPhoton, phiPhoton;
    size_t filterIndex = triggerSummary->filterIndex( triggerFilter_ );
    trigger::TriggerObjectCollection triggerObjects = triggerSummary->getObjects();
    if( !(filterIndex >= triggerSummary->sizeFilters()) ){
        const trigger::Keys& keys = triggerSummary->filterKeys( filterIndex );
        for( size_t j = 0; j < keys.size(); ++j ){
            trigger::TriggerObject foundObject = triggerObjects[keys[j]];
            if(fabs(foundObject.id()) == 22){ //It's a photon
			
				bool same= false;
				for(unsigned int x=0;x<ptPhoton.size();x++){
				  if(fabs(ptPhoton[x] - foundObject.pt()) < 0.01 || fabs(etaPhoton[x] - foundObject.eta()) < 0.001 || fabs(phiPhoton[x] - foundObject.phi()) < 0.001)
				    same = true;
				}
				
				if(!same){
				  h_triggerPhoPt->Fill(foundObject.pt());
				  h_triggerPhoEta->Fill(foundObject.eta());
				  h_triggerPhoPhi->Fill(foundObject.phi());
				  ptPhoton.push_back(foundObject.pt());
				  etaPhoton.push_back(foundObject.eta());
				  phiPhoton.push_back(foundObject.phi());
				}
            }
        }
        if (ptPhoton.size()>=2) {
            math::PtEtaPhiMLorentzVectorD* pho1 = new math::PtEtaPhiMLorentzVectorD(ptPhoton[0],etaPhoton[0],phiPhoton[0],0.000);
            math::PtEtaPhiMLorentzVectorD* pho2 = new math::PtEtaPhiMLorentzVectorD(ptPhoton[1],etaPhoton[1],phiPhoton[1],0.000);
            (*pho1)+=(*pho2);
            h_triggerDoublePhoMass->Fill(pho1->M());
            delete pho1;
            delete pho2;
        } else {
            h_triggerDoublePhoMass->Fill(-1.);
        }
    }
    
    
    bool hasFired = false;
    bool hasFiredAuxiliary = false;
    const edm::TriggerNames& trigNames = e.triggerNames(*hltresults);
    unsigned int numTriggers = trigNames.size();
    for( unsigned int hltIndex=0; hltIndex<numTriggers; ++hltIndex ){
        if (trigNames.triggerName(hltIndex).find(triggerPath_) != std::string::npos && hltresults->wasrun(hltIndex) && hltresults->accept(hltIndex)) hasFired = true;
        if (trigNames.triggerName(hltIndex).find(triggerPathAuxiliary_) != std::string::npos && hltresults->wasrun(hltIndex) && hltresults->accept(hltIndex)) hasFiredAuxiliary = true;
    }
    
    
    
    if(hasFiredAuxiliary) {
        
        //Matching the Photons
        int indexOfMatchedPhoton[2] = {-1};
        int matchedCounter = 0;
        int offlineCounter = 0;
        for(reco::PhotonCollection::const_iterator pho = PhotonCollection->begin(); (pho != PhotonCollection->end() && matchedCounter < 2) ; ++pho) {
            for(size_t off_i = 0; off_i < ptPhoton.size(); ++off_i) {
                if(sqrt((pho->phi()-phiPhoton[off_i])*(pho->phi()-phiPhoton[off_i]) + (pho->eta()-etaPhoton[off_i])*(pho->eta()-etaPhoton[off_i])) < 0.5) {
                    indexOfMatchedPhoton[matchedCounter] = offlineCounter;
                    matchedCounter++;
                    break;
                }
            }
            offlineCounter++;
        }
        
        
        if(hasFiredAuxiliary && PhotonCollection->size()>1) {
            if(hasFired && indexOfMatchedPhoton[1] >= 0) { //fill trailing leg
                h_PhoTurnOn_num-> Fill(PhotonCollection->at(indexOfMatchedPhoton[1]).pt());
            }
            h_PhoTurnOn_den-> Fill(PhotonCollection->at(1).pt());
        }
        
    }
}


void SUSY_HLT_DoublePhoton::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup)
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "SUSY_HLT_DoublePhoton::endLuminosityBlock" << std::endl;
}


void SUSY_HLT_DoublePhoton::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
    edm::LogInfo("SUSY_HLT_DoublePhoton") << "SUSY_HLT_DoublePhoton::endRun" << std::endl;
}

void SUSY_HLT_DoublePhoton::bookHistos(DQMStore::IBooker & ibooker_)
{
    ibooker_.cd();
    ibooker_.setCurrentFolder("HLT/SUSYBSM/" + triggerPath_);
    
    //offline quantities
    
    //online quantities
    h_triggerPhoPt = ibooker_.book1D("triggerPhoPt", "Trigger Photon Pt; GeV", 50, 0.0, 500.0);
    h_triggerPhoEta = ibooker_.book1D("triggerPhoEta", "Trigger Photon Eta", 20, -3.0, 3.0);
    h_triggerPhoPhi = ibooker_.book1D("triggerPhoPhi", "Trigger Photon Phi", 20, -3.5, 3.5);

    h_triggerDoublePhoMass = ibooker_.book1D("triggerDoublePhoMass", "Trigger Double Photon Mass", 202, -2, 200);

    //num and den hists to be divided in harvesting step to make turn on curves
    h_PhoTurnOn_num = ibooker_.book1D("PhoTurnOn_num", "Photon Turn On Numerator", 30, 0.0, 150 );
    h_PhoTurnOn_den = ibooker_.book1D("PhoTurnOn_den", "Photon Turn On Denominator", 30, 0.0, 150.0 );
    
    ibooker_.cd();
}

//define this as a plug-in
DEFINE_FWK_MODULE(SUSY_HLT_DoublePhoton);
