#include "HLTrigger/special/interface/HLTEcalPhiSymFilter.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"


HLTEcalPhiSymFilter::HLTEcalPhiSymFilter(const edm::ParameterSet& iConfig)
{
  barrelDigis_ = iConfig.getParameter< edm::InputTag > ("barrelDigiCollection");
  endcapDigis_ = iConfig.getParameter< edm::InputTag > ("endcapDigiCollection");
  barrelHits_ = iConfig.getParameter< edm::InputTag > ("barrelHitCollection");
  endcapHits_ = iConfig.getParameter< edm::InputTag > ("endcapHitCollection");
  phiSymBarrelDigis_ = 
    iConfig.getParameter< std::string > ("phiSymBarrelDigiCollection");
  phiSymEndcapDigis_ = 
    iConfig.getParameter< std::string > ("phiSymEndcapDigiCollection");
  phiSymBarrelHits_ = 
    iConfig.getParameter< std::string > ("phiSymBarrelHitCollection");
  phiSymEndcapHits_ = 
    iConfig.getParameter< std::string > ("phiSymEndcapHitCollection");
  eCut_barl_ = iConfig.getParameter< double > ("eCut_barrel");
  eCut_endc_ = iConfig.getParameter< double > ("eCut_endcap");
  eCut_barl_high_=iConfig.getParameter< double > ("eCut_barrel_high");
  eCut_endc_high_=iConfig.getParameter< double > ("eCut_endcap_high");

  statusThreshold_ = iConfig.getParameter<uint32_t> ("statusThreshold");
  useRecoFlag_ =  iConfig.getParameter<bool>("useRecoFlag");

  barrelDigisToken_ = consumes<EBDigiCollection>(barrelDigis_);
  endcapDigisToken_ = consumes<EEDigiCollection>(endcapDigis_);
  barrelHitsToken_ = consumes<EBRecHitCollection>(barrelHits_);
  endcapHitsToken_ = consumes<EERecHitCollection>(endcapHits_);

  //register your products
  produces< EBDigiCollection >(phiSymBarrelDigis_);
  produces< EEDigiCollection >(phiSymEndcapDigis_);
  produces< EBRecHitCollection >(phiSymBarrelHits_);
  produces< EERecHitCollection >(phiSymEndcapHits_);
}


HLTEcalPhiSymFilter::~HLTEcalPhiSymFilter()
{}

void
HLTEcalPhiSymFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("barrelDigiCollection",edm::InputTag("ecalDigis","ebDigis"));
  desc.add<edm::InputTag>("endcapDigiCollection",edm::InputTag("ecalDigis","eeDigis"));
  desc.add<edm::InputTag>("barrelHitCollection",edm::InputTag("ecalRecHit","EcalRecHitsEB"));
  desc.add<edm::InputTag>("endcapHitCollection",edm::InputTag("ecalRecHit","EcalRecHitsEE"));
  desc.add<unsigned int>("statusThreshold",3);
  desc.add<bool>("useRecoFlag",false);
  desc.add<double>("eCut_barrel",0.15);
  desc.add<double>("eCut_endcap",0.75);
  desc.add<double>("eCut_barrel_high",999999.);
  desc.add<double>("eCut_endcap_high",999999.);
  desc.add<std::string>("phiSymBarrelDigiCollection","phiSymEcalDigisEB");
  desc.add<std::string>("phiSymEndcapDigiCollection","phiSymEcalDigisEE");
  desc.add<std::string>("phiSymBarrelHitCollection","phiSymEcalRecHitsEB");
  desc.add<std::string>("phiSymEndcapHitCollection","phiSymEcalRecHitsEE");
  descriptions.add("alCaPhiSymStream",desc);
}


// ------------ method called to produce the data  ------------
bool
HLTEcalPhiSymFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  using namespace edm;
  using namespace std;
  
  edm::ESHandle<EcalChannelStatus> csHandle;
  if (! useRecoFlag_) iSetup.get<EcalChannelStatusRcd>().get(csHandle);
  const EcalChannelStatus& channelStatus = *csHandle; 
  

  Handle<EBDigiCollection> barrelDigisHandle;
  Handle<EEDigiCollection> endcapDigisHandle;
  Handle<EBRecHitCollection> barrelRecHitsHandle;
  Handle<EERecHitCollection> endcapRecHitsHandle;

  iEvent.getByToken(barrelDigisToken_,barrelDigisHandle);
  iEvent.getByToken(endcapDigisToken_,endcapDigisHandle);  
  iEvent.getByToken(barrelHitsToken_,barrelRecHitsHandle);
  iEvent.getByToken(endcapHitsToken_,endcapRecHitsHandle);
 
  //Create empty output collections
  std::auto_ptr< EBDigiCollection > phiSymEBDigiCollection( new EBDigiCollection );
  std::auto_ptr< EEDigiCollection > phiSymEEDigiCollection( new EEDigiCollection );
  std::auto_ptr< EBRecHitCollection > phiSymEBRecHitCollection( new EBRecHitCollection );
  std::auto_ptr< EERecHitCollection > phiSymEERecHitCollection( new EERecHitCollection );

  //Select interesting EcalRecHits (barrel)
  EBRecHitCollection::const_iterator itb;
  for (itb=barrelRecHitsHandle->begin(); itb!=barrelRecHitsHandle->end(); itb++) {
    EBDetId ebid(itb->id());
    if (itb->energy() >= eCut_barl_) {
      uint16_t statusCode = 0; 
      if (useRecoFlag_) statusCode=itb->recoFlag();
      else statusCode = channelStatus[itb->id().rawId()].getStatusCode();
      if ( statusCode <=statusThreshold_ ) { 
           phiSymEBRecHitCollection->push_back(*itb);
           EBDataFrame frameEB = (*barrelDigisHandle)[ebid.hashedIndex()];
           phiSymEBDigiCollection->push_back(frameEB.id(),frameEB.frame().begin());
      }
      else if (itb->energy() >= eCut_barl_high_ ) {
	   phiSymEBRecHitCollection->push_back(*itb);
           EBDataFrame frameEB = (*barrelDigisHandle)[ebid.hashedIndex()];
           phiSymEBDigiCollection->push_back(frameEB.id(),frameEB.frame().begin());
      }
    }
  }
  
  //Select interesting EcalRecHits (endcaps)
  EERecHitCollection::const_iterator ite;
  for (ite=endcapRecHitsHandle->begin(); ite!=endcapRecHitsHandle->end(); ite++) {
    EEDetId eeid( ite -> id() );
    if (ite->energy() >= eCut_endc_) {
       uint16_t statusCode = 0; 
       if (useRecoFlag_) statusCode=ite->recoFlag();
       else statusCode =channelStatus[ite->id().rawId()].getStatusCode(); 
       if ( statusCode <=statusThreshold_ ) {
            phiSymEERecHitCollection->push_back(*ite);
            EEDataFrame frameEE = (*endcapDigisHandle)[eeid.hashedIndex()];
            phiSymEEDigiCollection->push_back(frameEE.id(),frameEE.frame().begin());
       }
       else if (ite->energy() >= eCut_endc_high_ ) {
	    phiSymEERecHitCollection->push_back(*ite);
            EEDataFrame frameEE = (*endcapDigisHandle)[eeid.hashedIndex()];
            phiSymEEDigiCollection->push_back(frameEE.id(),frameEE.frame().begin());
       }
    }
  }

  if ((!phiSymEBDigiCollection->size() ) && (!phiSymEEDigiCollection->size())) 
    return false;
  if ((!phiSymEBRecHitCollection->size() ) && (!phiSymEERecHitCollection->size())) 
    return false;
  
  //Put selected information in the event
  iEvent.put( phiSymEBDigiCollection, phiSymBarrelDigis_);
  iEvent.put( phiSymEEDigiCollection, phiSymEndcapDigis_);
  iEvent.put( phiSymEBRecHitCollection, phiSymBarrelHits_);
  iEvent.put( phiSymEERecHitCollection, phiSymEndcapHits_);
  
  return true;
}
