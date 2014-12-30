# Auto generated configuration file
# using:
# Revision: 1.381.2.13
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v
# with command line options: data -s RAW2DIGI,L1Reco,RECO --eventcontent RECO --conditions auto:com10 -n 500 --no_exec --data --scenario pp --process RERECO
import FWCore.ParameterSet.Config as cms

process = cms.Process('RERECO')

#
process = cms.Process("HistoEnergy")
#

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
                                       input = cms.untracked.int32(100)
                                       )

# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
                                                              		'root://eoscms//eos/cms/store/data/Run2012A/MinimumBias/RAW/v1/000/191/226/FA549004-0886-E111-AB6F-BCAEC518FF6E.root',
                                                            
                                                              )
                            
                            #process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False)
                            
                            )

# Production Info
process.configurationMetadata = cms.untracked.PSet(
                                                   version = cms.untracked.string('$Revision: 1.381.2.13 $'),
                                                   annotation = cms.untracked.string('data nevts:500'),
                                                   name = cms.untracked.string('PyReleaseValidation')
                                                   )

#HLTStream

process.hltAlCaPhiSymStream = cms.EDFilter( "HLTEcalPhiSymFilter",
    statusThreshold = cms.uint32( 3 ),
    barrelDigiCollection = cms.InputTag('ecalDigis','ebDigis'),
    endcapDigiCollection = cms.InputTag('ecalDigis','eeDigis'),
    barrelUncalibHitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB"),
    endcapUncalibHitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE"),
    barrelHitCollection = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
    endcapHitCollection = cms.InputTag('ecalRecHit','EcalRecHitsEE'),
    ampCutADC_barrel = cms.double( 4.5 ),
    ampCutADC_endcap = cms.double( 7.0 ),
    eCut_endcap_high = cms.double( 999999.0 ),
    eCut_barrel = cms.double( 0.15 ),
    eCut_barrel_high = cms.double( 999999.0 ),
    eCut_endcap = cms.double( 0.75 ),
    useRecoFlag = cms.bool( False ),
    phiSymBarrelDigiCollection = cms.string( "phiSymEcalDigisEB" ),
    phiSymEndcapDigiCollection = cms.string( "phiSymEcalDigisEE" ),
    phiSymBarrelHitCollection = cms.string( "phiSymEcalRecHitsEB" ),
    phiSymEndcapHitCollection = cms.string( "phiSymEcalRecHitsEE" )
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('AlCaPhiSym_stream_RAW2DIGI_RECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)
                       
#
# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:com10', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_73_V0A::All', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction*process.hltAlCaPhiSymStream)
#process.hltAlCaPhiSymStream_step = cms.Path(process.hltAlCaPhiSymStream)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
process.RECOSIMoutput.outputCommands.append('keep *')
#process.RECOSIMoutput.outputCommands.append('keep phiSymEcalDigisEB_*_*_*')
#process.RECOSIMoutput.outputCommands.append('keep phiSymEcalDigisEE_*_*_*')
#process.RECOSIMoutput.outputCommands.append('keep phiSymEcalRecHitsEB_*_*_*')
#process.RECOSIMoutput.outputCommands.append('keep phiSymEcalRecHitsEE_*_*_*')

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

