import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9

process = cms.Process("SaveNtuple",Phase2C17I13M9)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtendedRun4D110Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T33', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("training_ntuple_test.root"),
    closeFileFast = cms.untracked.bool(True)
)


# process.TFileService = cms.Service("TFileService",
#     fileName = cms.string("training_ntuple_9834.0_TTbar_14TeV+Run4D110PU_default.root"),
#     closeFileFast = cms.untracked.bool(True)
# )

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/relval/CMSSW_15_0_0_pre2/RelValTTbar_14TeV/GEN-SIM-RECO/PU_141X_mcRun4_realistic_v3_STD_Run4D110_PU-v1/2590000/067640ef-59c9-42de-b5c4-f90c7343458c.root')  # Replace with your input file
#)

#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(
#                               '/store/relval/CMSSW_15_0_0_pre2/RelValTTbar_14TeV/GEN-SIM-RECO/PU_141X_mcRun4_realistic_v3_STD_Run4D110_PU-v1/2590000/067640ef-59c9-42de-b5c4-f90c7343458c.root'
#                            )
#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring('file:step3_for_testing.root')
#)

# process.source = cms.Source("PoolSource",
#                             fileNames = cms.untracked.vstring('file:/eos/user/m/mklevs/CMSSW_15_0_0_pre2/src/RecoMTD/TimingIDTools/test/full_pipeline/29834.0_TTbar_14TeV+Run4D110PU_default/step3.root')
# )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_15_1_0_pre2/RelValTTbar_14TeV/GEN-SIM-RECO/PU_150X_mcRun4_realistic_v1_STD_Run4D110_PU-v1/2580000/0653bad3-d647-4d0c-8980-ff432398270f.root')  # Replace with your input file
)

# 'file:/eos/user/m/mklevs/CMSSW_15_0_0_pre2/src/RecoMTD/TimingIDTools/test/full_pipeline/29834.0_TTbar_14TeV+Run4D110PU_default/step2.root'

from RecoMTD.TimingIDTools.mvaTrainingNtuple_cff import mvaTrainingNtuple

process.mvaTrainingNtuple = mvaTrainingNtuple

process.p = cms.Path(process.mvaTrainingNtuple)

