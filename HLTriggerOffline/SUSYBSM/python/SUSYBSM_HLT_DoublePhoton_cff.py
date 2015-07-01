import FWCore.ParameterSet.Config as cms

SUSY_HLT_DoublePhoton = cms.EDAnalyzer("SUSY_HLT_DoublePhoton",
  trigSummary = cms.InputTag("hltTriggerSummaryAOD",'', 'HLT'), #to use with test sample
  #trigSummary = cms.InputTag("hltTriggerSummaryAOD"),
  PhotonCollection = cms.InputTag("photons"),
  TriggerResults = cms.InputTag('TriggerResults','','HLT'), #to use with test sample
  #TriggerResults = cms.InputTag('TriggerResults','','HLT'),
  HLTProcess = cms.string('HLT'),
  TriggerPath = cms.string('HLT_DoublePhoton85_v'),
  TriggerPathAuxiliary = cms.string('HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v'),
  TriggerFilter = cms.InputTag('hltL1sL1SingleEG40ORL1SingleEG35ORL1SingleJet200', '', 'HLT'), #the last filter in the path
)



SUSY_HLT_DoublePhoton_FASTSIM = cms.EDAnalyzer("SUSY_HLT_DoublePhoton",
  trigSummary = cms.InputTag("hltTriggerSummaryAOD",'', 'HLT'), #to use with test sample
  #trigSummary = cms.InputTag("hltTriggerSummaryAOD"),
  PhotonCollection = cms.InputTag("photons"),
  TriggerResults = cms.InputTag('TriggerResults','','HLT'), #to use with test sample
  #TriggerResults = cms.InputTag('TriggerResults','','HLT'),
  HLTProcess = cms.string('HLT'),
  TriggerPath = cms.string('HLT_DoublePhoton85_v'),
  TriggerPathAuxiliary = cms.string('HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v'),
  TriggerFilter = cms.InputTag('hltL1sL1SingleEG40ORL1SingleEG35ORL1SingleJet200', '', 'HLT'), #the last filter in the path   
)

SUSY_HLT_DoublePhoton_POSTPROCESSING = cms.EDAnalyzer("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/SUSYBSM/HLT_DoublePhoton85_v"),
    verbose        = cms.untracked.uint32(2), # Set to 2 for all messages
    resolution     = cms.vstring(""),
    efficiency     = cms.vstring("PhoTurnOn_eff 'Turn-on vs Photon pT; pT (GeV); #epsilon' PhoTurnOn_num PhoTurnOn_den")
)


SUSY_HLT_DoublePhoton_FASTSIM_POSTPROCESSING = cms.EDAnalyzer("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/SUSYBSM/HLT_DoublePhoton85_v"),
    verbose        = cms.untracked.uint32(2), # Set to 2 for all messages
    resolution     = cms.vstring(""),
    efficiency     = cms.vstring("PhoTurnOn_eff 'Turn-on vs Photon pT; pT (GeV); #epsilon' PhoTurnOn_num PhoTurnOn_den")
)




