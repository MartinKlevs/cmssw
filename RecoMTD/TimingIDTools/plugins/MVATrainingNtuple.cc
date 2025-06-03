#include <string>
#include <memory>
#include "TTree.h"
#include "TFile.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
// #include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "DataFormats/Common/interface/ValueMap.h"

// #include "DQMServices/Core/interface/DQMEDAnalyzer.h"
// #include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/ValidHandle.h"
// #include "DataFormats/Math/interface/deltaR.h"
// #include "DataFormats/Math/interface/GeantUnits.h"
// #include "DataFormats/Math/interface/angle_units.h"
#include "DataFormats/ForwardDetId/interface/ETLDetId.h"
#include "DataFormats/ForwardDetId/interface/BTLDetId.h"

// #include "DataFormats/Common/interface/Ptr.h"
// #include "DataFormats/Common/interface/PtrVector.h"
// #include "DataFormats/Common/interface/RefProd.h"
// #include "DataFormats/Common/interface/Ref.h"
// #include "DataFormats/Common/interface/RefVector.h"

// #include "DataFormats/TrackReco/interface/Track.h"
// #include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Geometry/Records/interface/MTDDigiGeometryRecord.h"
#include "Geometry/Records/interface/MTDTopologyRcd.h"
#include "Geometry/MTDGeometryBuilder/interface/MTDTopology.h"
#include "Geometry/MTDCommonData/interface/MTDTopologyMode.h"
#include "Geometry/MTDGeometryBuilder/interface/MTDGeometry.h"
#include "Geometry/MTDGeometryBuilder/interface/ProxyMTDTopology.h"
#include "Geometry/MTDGeometryBuilder/interface/RectangularMTDTopology.h"

// #include "RecoMTD/DetLayers/interface/MTDDetLayerGeometry.h"
// #include "RecoMTD/Records/interface/MTDRecoGeometryRecord.h"
// #include "MagneticField/Engine/interface/MagneticField.h"
// #include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
// #include "RecoMTD/DetLayers/interface/MTDDetLayerGeometry.h"
// #include "RecoMTD/DetLayers/interface/MTDTrayBarrelLayer.h"
// #include "RecoMTD/DetLayers/interface/MTDDetTray.h"
// #include "RecoMTD/DetLayers/interface/MTDSectorForwardDoubleLayer.h"
// #include "RecoMTD/DetLayers/interface/MTDDetSector.h"
// #include "RecoMTD/Records/interface/MTDRecoGeometryRecord.h"
#include "Geometry/MTDGeometryBuilder/interface/MTDGeomUtil.h"

// #include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
// #include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
// #include "TrackingTools/PatternTools/interface/Trajectory.h"
// #include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
// #include "TrackingTools/Records/interface/TransientTrackRecord.h"
// #include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
// #include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
// #include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/FTLRecHit/interface/FTLRecHitCollections.h"
#include "DataFormats/FTLRecHit/interface/FTLClusterCollections.h"
#include "DataFormats/TrackerRecHit2D/interface/MTDTrackingRecHit.h"

// #include "DataFormats/Common/interface/OneToMany.h"
// #include "DataFormats/Common/interface/AssociationMap.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/Associations/interface/TrackToTrackingParticleAssociator.h"
// #include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
// #include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
// #include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/Associations/interface/MtdSimLayerClusterToTPAssociatorBaseImpl.h"
// #include "SimDataFormats/CaloAnalysis/interface/MtdSimLayerCluster.h"
#include "SimDataFormats/Associations/interface/MtdRecoClusterToSimLayerClusterAssociationMap.h"
#include "SimDataFormats/Associations/interface/MtdSimLayerClusterToRecoClusterAssociationMap.h"

#include "RecoVertex/PrimaryVertexProducer/interface/HITrackFilterForPVFinding.h"

// #include "CLHEP/Units/PhysicalConstants.h"
// #include "MTDHit.h"

class MVATrainingNtuple : public edm::one::EDAnalyzer<edm::one::SharedResources> {
typedef math::XYZTLorentzVector LorentzVector;

public:
  explicit MVATrainingNtuple(const edm::ParameterSet&);
  ~MVATrainingNtuple() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  // const bool trkTPSelLV(const TrackingParticle&);
  const bool trkTPSelAll(const TrackingParticle&);
  const bool trkRecSel(const reco::TrackBase&);
  // const bool trkRecSelLowPt(const reco::TrackBase&);
  const edm::Ref<std::vector<TrackingParticle>>* getMatchedTP(const reco::TrackBaseRef&);

  edm::Service<TFileService> fs_;


  // ------------ member data ------------

  TrackFilterForPVFindingBase* theTrackFilter;
  const std::string folder_;
  // const float trackMaxPt_;
  const float trackMaxBtlEta_;
  const float trackMinEtlEta_;
  const float trackMaxEtlEta_;

  static constexpr double simUnit_ = 1e9;                // sim time in s while reco time in ns
  static constexpr double etacutGEN_ = 4.;               // |eta| < 4;
  static constexpr double etacutREC_ = 3.;               // |eta| < 3;
  static constexpr double pTcutBTL_ = 0.7;               // PT > 0.7 GeV
  static constexpr double pTcutETL_ = 0.2;               // PT > 0.2 GeV
  // static constexpr double depositBTLthreshold_ = 1;      // threshold for energy deposit in BTL cell [MeV]
  // static constexpr double depositETLthreshold_ = 0.001;  // threshold for energy deposit in ETL cell [MeV]
  static constexpr double rBTL_ = 110.0;
  static constexpr double zETL_ = 290.0;
  // static constexpr double etaMatchCut_ = 0.05;
  // static constexpr double cluDRradius_ = 0.05;  // to cluster rechits around extrapolated track

  const reco::RecoToSimCollection* r2s_;

    // BDT input variables
    std::vector<double> Ttrack_pt, Ttrack_eta, Ttrack_phi, Ttrack_dz, Ttrack_dxy,
      // Ttrack_zPCA,
      Ttrack_chi2, Ttrack_BTLchi2, Ttrack_BTLtime_chi2, Ttrack_ETLchi2, Ttrack_ETLtime_chi2,
      Ttrack_t0, Ttrack_t0pid, Ttrack_t0safe, Ttrack_Tmtd,
      // Ttrack_tPi, Ttrack_tK, Ttrack_tP,
      Ttrack_sigmat0, Ttrack_sigmaTmtd,
      // Ttrack_sigma_t0safe,
      // Ttrack_sigma_tPi, Ttrack_sigma_tK, Ttrack_sigma_tP,
      // Ttrack_probPi, Ttrack_probK, Ttrack_probP,
      Ttrack_length, Ttrack_MtdMVA, Ttrack_lHitPos,
      // gnn_sim_vertex_z, gnn_sim_vertex_t, gnn_tp_tEst, gnn_outermostHitPosition,
      TtrackTP_pt, TtrackTP_eta, TtrackTP_phi;
    std::vector<int> Ttrack_ndof, Ttrack_nValidHits, Ttrack_npixBarrelValidHits, Ttrack_npixEndcapValidHits,
      TtrackTP_nValidHits;
    std::vector<bool> Ttrack_Signal, Ttrack_Associated, Ttrack_DirClu, Ttrack_OtherClu,
      Ttrack_isBTL, Ttrack_isETL, Ttrack_ETLdisc1, Ttrack_ETLdisc2;

  edm::EDGetTokenT<reco::TrackCollection> GenRecTrackToken_;
  edm::EDGetTokenT<reco::TrackCollection> RecTrackToken_;

  // edm::EDGetTokenT<TrackingParticleCollection> trackingParticleCollectionToken_;
  // edm::EDGetTokenT<reco::SimToRecoCollection> simToRecoAssociationToken_;
  edm::EDGetTokenT<reco::RecoToSimCollection> recoToSimAssociationToken_;
  edm::EDGetTokenT<reco::TPToSimCollectionMtd> tp2SimAssociationMapToken_;
  edm::EDGetTokenT<reco::SimToTPCollectionMtd> Sim2tpAssociationMapToken_;
  edm::EDGetTokenT<MtdRecoClusterToSimLayerClusterAssociationMap> r2sAssociationMapToken_;

  // edm::EDGetTokenT<FTLRecHitCollection> btlRecHitsToken_;
  // edm::EDGetTokenT<FTLRecHitCollection> etlRecHitsToken_;
  edm::EDGetTokenT<FTLClusterCollection> btlRecCluToken_;
  edm::EDGetTokenT<FTLClusterCollection> etlRecCluToken_;

  edm::EDGetTokenT<edm::ValueMap<int>> trackAssocToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> pathLengthToken_;

  edm::EDGetTokenT<edm::ValueMap<float>> btlMatchTimeChi2Token_;
  edm::EDGetTokenT<edm::ValueMap<float>> etlMatchTimeChi2Token_;
  edm::EDGetTokenT<edm::ValueMap<float>> btlMatchChi2Token_;
  edm::EDGetTokenT<edm::ValueMap<float>> etlMatchChi2Token_;

  // edm::EDGetTokenT<edm::ValueMap<float>> sigmatmtdToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> t0SrcToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmat0SrcToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> t0PidToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmat0PidToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> t0SafePidToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmat0SafePidToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmaTofPiToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmaTofKToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmaTofPToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> tmtdToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> tofPiToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> tofKToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> tofPToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> probPiToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> probKToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> probPToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> trackMVAQualToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> outermostHitPositionToken_;

  // edm::EDGetTokenT<std::vector<reco::Vertex>> RecVertexToken_;
  edm::EDGetTokenT<reco::BeamSpot> RecBeamSpotToken_;
  edm::EDGetTokenT<TrackingVertexCollection> trackingVertexCollectionToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> momentumToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> sigmatimeToken_;

  edm::EDGetTokenT<edm::ValueMap<int>> npixBarrelToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> npixEndcapToken_;

  edm::ESGetToken<MTDGeometry, MTDDigiGeometryRecord> mtdgeoToken_;
  edm::ESGetToken<MTDTopology, MTDTopologyRcd> mtdtopoToken_;
  // edm::ESGetToken<MTDDetLayerGeometry, MTDRecoGeometryRecord> mtdlayerToken_;
  // edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magfieldToken_;
  // edm::ESGetToken<TransientTrackBuilder, TransientTrackRecord> builderToken_;
};

// ------------ constructor and destructor --------------
MVATrainingNtuple::MVATrainingNtuple(const edm::ParameterSet& iConfig)
    : folder_(iConfig.getParameter<std::string>("folder")),
      // trackMaxPt_(iConfig.getParameter<double>("trackMaximumPt")),
      trackMaxBtlEta_(iConfig.getParameter<double>("trackMaximumBtlEta")),
      trackMinEtlEta_(iConfig.getParameter<double>("trackMinimumEtlEta")),
      trackMaxEtlEta_(iConfig.getParameter<double>("trackMaximumEtlEta")) {
  // RecVertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("inputTagV"));
  RecBeamSpotToken_ = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("offlineBS"));
  trackingVertexCollectionToken_ = consumes<TrackingVertexCollection>(iConfig.getParameter<edm::InputTag>("SimTag"));
  momentumToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("momentumSrc"));
  sigmatimeToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmaSrc"));
  tofPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofPi"));
  tofKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofK"));
  tofPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofP"));
  probPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probPi"));
  probKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probK"));
  probPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probP"));
  sigmaTofPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofpiSrc"));
  sigmaTofKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofkSrc"));
  sigmaTofPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofpSrc"));
  btlMatchChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("btlMatchChi2Src"));
  etlMatchChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("etlMatchChi2Src"));
  npixBarrelToken_ = consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("npixBarrelSrc"));
  npixEndcapToken_ = consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("npixEndcapSrc"));


  GenRecTrackToken_ = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("inputTagG"));
  RecTrackToken_ = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("inputTagT"));
  // trackingParticleCollectionToken_ =
  //     consumes<TrackingParticleCollection>(iConfig.getParameter<edm::InputTag>("SimTag"));
  // simToRecoAssociationToken_ =
  //     consumes<reco::SimToRecoCollection>(iConfig.getParameter<edm::InputTag>("TPtoRecoTrackAssoc"));
  recoToSimAssociationToken_ =
      consumes<reco::RecoToSimCollection>(iConfig.getParameter<edm::InputTag>("TPtoRecoTrackAssoc"));
  tp2SimAssociationMapToken_ =
      consumes<reco::TPToSimCollectionMtd>(iConfig.getParameter<edm::InputTag>("tp2SimAssociationMapTag"));
  Sim2tpAssociationMapToken_ =
      consumes<reco::SimToTPCollectionMtd>(iConfig.getParameter<edm::InputTag>("Sim2tpAssociationMapTag"));
  r2sAssociationMapToken_ = consumes<MtdRecoClusterToSimLayerClusterAssociationMap>(
      iConfig.getParameter<edm::InputTag>("r2sAssociationMapTag"));
  // btlRecHitsToken_ = consumes<FTLRecHitCollection>(iConfig.getParameter<edm::InputTag>("btlRecHits"));
  // etlRecHitsToken_ = consumes<FTLRecHitCollection>(iConfig.getParameter<edm::InputTag>("etlRecHits"));
  btlRecCluToken_ = consumes<FTLClusterCollection>(iConfig.getParameter<edm::InputTag>("recCluTagBTL"));
  etlRecCluToken_ = consumes<FTLClusterCollection>(iConfig.getParameter<edm::InputTag>("recCluTagETL"));
  trackAssocToken_ = consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("trackAssocSrc"));
  pathLengthToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("pathLengthSrc"));
  btlMatchTimeChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("btlMatchTimeChi2"));
  etlMatchTimeChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("etlMatchTimeChi2"));
  btlMatchChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("btlMatchChi2"));
  tmtdToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tmtd"));
  // sigmatmtdToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatmtd"));
  t0SrcToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("t0Src"));
  sigmat0SrcToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmat0Src"));
  t0PidToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("t0PID"));
  sigmat0PidToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmat0PID"));
  t0SafePidToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("t0SafePID"));
  sigmat0SafePidToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmat0SafePID"));
  sigmaTofPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmaTofPi"));
  sigmaTofKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmaTofK"));
  sigmaTofPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmaTofP"));
  trackMVAQualToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("trackMVAQual"));
  outermostHitPositionToken_ =
      consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("outermostHitPositionSrc"));
  mtdgeoToken_ = esConsumes<MTDGeometry, MTDDigiGeometryRecord>();
  mtdtopoToken_ = esConsumes<MTDTopology, MTDTopologyRcd>();
  // mtdlayerToken_ = esConsumes<MTDDetLayerGeometry, MTDRecoGeometryRecord>();
  // magfieldToken_ = esConsumes<MagneticField, IdealMagneticFieldRecord>();
  // builderToken_ = esConsumes<TransientTrackBuilder, TransientTrackRecord>(edm::ESInputTag("", "TransientTrackBuilder"));

  // select and configure the track selection
  std::string trackSelectionAlgorithm =
      iConfig.getParameter<edm::ParameterSet>("TkFilterParameters").getParameter<std::string>("algorithm");
  if (trackSelectionAlgorithm == "filter") {
    theTrackFilter = new TrackFilterForPVFinding(iConfig.getParameter<edm::ParameterSet>("TkFilterParameters"));
  } else if (trackSelectionAlgorithm == "filterWithThreshold") {
    theTrackFilter = new HITrackFilterForPVFinding(iConfig.getParameter<edm::ParameterSet>("TkFilterParameters"));
  } else {
    edm::LogWarning("MVATrainingNtuple: unknown track selection algorithm: " + trackSelectionAlgorithm);
  }
}

MVATrainingNtuple::~MVATrainingNtuple() {}

// ------------ method called for each event  ------------
void MVATrainingNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  // using namespace geant_units::operators;
  using namespace std;

  reco::BeamSpot beamSpot;
  edm::Handle<reco::BeamSpot> BeamSpotH;
  iEvent.getByToken(RecBeamSpotToken_, BeamSpotH);
  if (!BeamSpotH.isValid())
    edm::LogWarning("MVATrainingNtuple") << "BeamSpotH is not valid";
  beamSpot = *BeamSpotH;

  const auto& trackAssoc = iEvent.get(trackAssocToken_);

  auto GenRecTrackHandle = makeValid(iEvent.getHandle(GenRecTrackToken_));

  auto btlRecCluHandle = makeValid(iEvent.getHandle(btlRecCluToken_));
  auto etlRecCluHandle = makeValid(iEvent.getHandle(etlRecCluToken_));

  // std::unordered_map<uint32_t, MTDHit> m_btlHits;
  // std::unordered_map<uint32_t, MTDHit> m_etlHits;
  // std::unordered_map<uint32_t, std::set<unsigned long int>> m_btlTrkPerCell;
  // std::unordered_map<uint32_t, std::set<unsigned long int>> m_etlTrkPerCell;
  const auto& tp2SimAssociationMap = iEvent.get(tp2SimAssociationMapToken_);
  const auto& Sim2tpAssociationMap = iEvent.get(Sim2tpAssociationMapToken_);
  const auto& r2sAssociationMap = iEvent.get(r2sAssociationMapToken_);

  // const auto& tMtd = iEvent.get(tmtdToken_);
  // // const auto& sigmatMtd = iEvent.get(sigmatmtdToken_);
  // const auto& sigmatimemtd = iEvent.get(sigmatimeToken_);
  // const auto& t0Src = iEvent.get(t0SrcToken_);
  // const auto& sigmat0Src = iEvent.get(sigmat0SrcToken_);
  // const auto& t0Pid = iEvent.get(t0PidToken_);
  // const auto& sigmat0Pid = iEvent.get(sigmat0PidToken_);
  // const auto& t0Safe = iEvent.get(t0SafePidToken_);
  // const auto& sigmat0Safe = iEvent.get(sigmat0SafePidToken_);
  // const auto& sigmaTofPi = iEvent.get(sigmaTofPiToken_);
  // const auto& sigmaTofK = iEvent.get(sigmaTofKToken_);
  // const auto& sigmaTofP = iEvent.get(sigmaTofPToken_);
  // const auto& mtdQualMVA = iEvent.get(trackMVAQualToken_);
  // const auto& trackAssoc = iEvent.get(trackAssocToken_);
  // const auto& pathLength = iEvent.get(pathLengthToken_);
  // const auto& btlMatchTimeChi2 = iEvent.get(btlMatchTimeChi2Token_);
  // const auto& etlMatchTimeChi2 = iEvent.get(etlMatchTimeChi2Token_);
  // const auto& btlMatchChi2 = iEvent.get(btlMatchChi2Token_);
  // const auto& outermostHitPosition = iEvent.get(outermostHitPositionToken_);

  const auto& btlMatchChi2 = iEvent.get(btlMatchChi2Token_);
  const auto& btlMatchTimeChi2 = iEvent.get(btlMatchTimeChi2Token_);
  const auto& etlMatchChi2 = iEvent.get(etlMatchChi2Token_);
  const auto& etlMatchTimeChi2 = iEvent.get(etlMatchTimeChi2Token_);
  // const auto& momentum = iEvent.get(momentumToken_);
  const auto& mtdQualMVA = iEvent.get(trackMVAQualToken_);
  const auto& npixBarrel = iEvent.get(npixBarrelToken_);
  const auto& npixEndcap = iEvent.get(npixEndcapToken_);
  const auto& outermostHitPosition = iEvent.get(outermostHitPositionToken_);
  const auto& pathLength = iEvent.get(pathLengthToken_);
  // const auto& probK = iEvent.get(probKToken_);
  // const auto& probP = iEvent.get(probPToken_);
  // const auto& probPi = iEvent.get(probPiToken_);
  const auto& sigmat0PID = iEvent.get(sigmat0PidToken_);
  const auto& sigmat0Safe = iEvent.get(sigmat0SafePidToken_);
  const auto& sigmat0Src = iEvent.get(sigmat0SrcToken_);
  const auto& sigmatimemtd = iEvent.get(sigmatimeToken_);
  // const auto& sigmatofk = iEvent.get(sigmaTofKToken_);
  // const auto& sigmatofp = iEvent.get(sigmaTofPToken_);
  // const auto& sigmatofpi = iEvent.get(sigmaTofPiToken_);
  // const auto& t0Pid = iEvent.get(t0PidToken_);
  // const auto& t0Safe = iEvent.get(t0SafePidToken_);
  const auto& t0Src = iEvent.get(t0SrcToken_);
  const auto& tMtd = iEvent.get(tmtdToken_);
  // const auto& tofK = iEvent.get(tofKToken_);
  // const auto& tofP = iEvent.get(tofPToken_);
  // const auto& tofPi = iEvent.get(tofPiToken_);

  auto recoToSimH = makeValid(iEvent.getHandle(recoToSimAssociationToken_));
  r2s_ = recoToSimH.product();
  auto geometryHandle = iSetup.getTransientHandle(mtdgeoToken_);
  const MTDGeometry* geom = geometryHandle.product();
  auto topologyHandle = iSetup.getTransientHandle(mtdtopoToken_);
  const MTDTopology* topology = topologyHandle.product();

  mtd::MTDGeomUtil geomUtil;
  geomUtil.setGeometry(geom);
  geomUtil.setTopology(topology);

  std::string BDTtreeName = "BDTtree_" + std::to_string(iEvent.id().event());
  TTree* BDTtree = fs_->make<TTree>(BDTtreeName.c_str(), "Tree for BDT tracks");

  BDTtree->Branch("Track_pt", &Ttrack_pt);
  BDTtree->Branch("Track_eta", &Ttrack_eta);
  BDTtree->Branch("Track_phi", &Ttrack_phi);
  BDTtree->Branch("Track_dz", &Ttrack_dz);
  BDTtree->Branch("Track_dxy", &Ttrack_dxy);
  BDTtree->Branch("Track_chi2", &Ttrack_chi2);
  BDTtree->Branch("Track_ndof", &Ttrack_ndof);
  BDTtree->Branch("Track_nValidHits", &Ttrack_nValidHits);
  // BDTtree->Branch("TrackTP_pt", &TtrackTP_pt);
  // BDTtree->Branch("TrackTP_eta", &TtrackTP_eta);
  // BDTtree->Branch("TrackTP_phi", &TtrackTP_phi);
  // BDTtree->Branch("TrackTP_nValidHits", &TtrackTP_nValidHits);
  BDTtree->Branch("Track_npixBarrelValidHits", &Ttrack_npixBarrelValidHits);
  BDTtree->Branch("Track_npixEndcapValidHits", &Ttrack_npixEndcapValidHits);
  BDTtree->Branch("Track_Signal", &Ttrack_Signal);
  BDTtree->Branch("Track_Associated", &Ttrack_Associated);
  BDTtree->Branch("Track_BTLchi2", &Ttrack_BTLchi2);
  BDTtree->Branch("Track_BTLtime_chi2", &Ttrack_BTLtime_chi2);
  BDTtree->Branch("Track_ETLchi2", &Ttrack_ETLchi2);
  BDTtree->Branch("Track_ETLtime_chi2", &Ttrack_ETLtime_chi2);
  BDTtree->Branch("Track_t0", &Ttrack_t0);
  BDTtree->Branch("Track_sigmat0", &Ttrack_sigmat0);
  BDTtree->Branch("Track_Tmtd", &Ttrack_Tmtd);
  BDTtree->Branch("Track_MtdMVA", &Ttrack_MtdMVA);
  BDTtree->Branch("Track_lHitPos", &Ttrack_lHitPos);
  BDTtree->Branch("Track_sigmaTmtd", &Ttrack_sigmaTmtd);
  BDTtree->Branch("Track_lenght", &Ttrack_length);
  BDTtree->Branch("Track_DirClu", &Ttrack_DirClu);
  BDTtree->Branch("Track_OtherClu", &Ttrack_OtherClu);
  BDTtree->Branch("Track_isBTL", &Ttrack_isBTL);
  BDTtree->Branch("Track_isETL", &Ttrack_isETL);
  BDTtree->Branch("Track_isETLD1", &Ttrack_ETLdisc1);
  BDTtree->Branch("Track_isETLD2", &Ttrack_ETLdisc2);

  Ttrack_pt.clear();
  Ttrack_eta.clear();
  Ttrack_phi.clear();
  Ttrack_dz.clear();
  Ttrack_dxy.clear();
  Ttrack_chi2.clear();
  Ttrack_ndof.clear();
  Ttrack_nValidHits.clear();
  // TtrackTP_pt.clear();
  // TtrackTP_eta.clear();
  // TtrackTP_phi.clear();
  // TtrackTP_nValidHits.clear();
  Ttrack_npixBarrelValidHits.clear();
  Ttrack_npixEndcapValidHits.clear();
  Ttrack_Signal.clear();
  Ttrack_Associated.clear();
  Ttrack_BTLchi2.clear();
  Ttrack_BTLtime_chi2.clear();
  Ttrack_ETLchi2.clear();
  Ttrack_ETLtime_chi2.clear();
  Ttrack_t0.clear();
  Ttrack_sigmat0.clear();
  Ttrack_Tmtd.clear();
  Ttrack_MtdMVA.clear();
  Ttrack_lHitPos.clear();
  Ttrack_sigmaTmtd.clear();
  Ttrack_length.clear();
  Ttrack_DirClu.clear();
  Ttrack_OtherClu.clear();
  Ttrack_isBTL.clear();
  Ttrack_isETL.clear();
  Ttrack_ETLdisc1.clear();
  Ttrack_ETLdisc2.clear();

  unsigned int index = 0;

  unsigned int nSelectedTracks1 = 0;
  unsigned int nSelectedTracks2 = 0;
  unsigned int nSelectedTracks3 = 0;

  // --- Loop over all RECO tracks ---
  for (const auto& trackGen : *GenRecTrackHandle) {
    nSelectedTracks1++;
    const reco::TrackRef trackref(iEvent.getHandle(GenRecTrackToken_), index);
    index++;

    if (trackAssoc[trackref] == -1) {
      LogWarning("mtdTracks") << "Extended track not associated";
      continue;
    }

    const reco::TrackRef mtdTrackref = reco::TrackRef(iEvent.getHandle(RecTrackToken_), trackAssoc[trackref]);
    const reco::Track& track = *mtdTrackref;

    bool isBTL = false;
    bool isETL = false;
    bool ETLdisc1 = false;
    bool ETLdisc2 = false;
    bool twoETLdiscs = false;
    bool isDirClu   = false;
    bool isOtherClu = false;

    // bool noCrack = std::abs(trackGen.eta()) < trackMaxBtlEta_ || std::abs(trackGen.eta()) > trackMinEtlEta_;

    nSelectedTracks2++;

    if (trkRecSel(trackGen)) {
      nSelectedTracks3++;
      if (std::round(sigmatimemtd[trackref] - sigmat0PID[trackref]) != 0) {
        LogWarning("mtdTracks")
            << "TimeError associated to refitted track is different from TimeError stored in tofPID "
               "sigmat0 ValueMap: this should not happen";
      }

      bool MTDEtlZnegD1 = false;
      bool MTDEtlZnegD2 = false;
      bool MTDEtlZposD1 = false;
      bool MTDEtlZposD2 = false;
      std::vector<edm::Ref<edmNew::DetSetVector<FTLCluster>, FTLCluster>> recoClustersRefs;

      if (std::abs(trackGen.eta()) < trackMaxBtlEta_) {
        // --- all BTL tracks (with and without hit in MTD) ---

        bool MTDBtl = false;
        int numMTDBtlvalidhits = 0;
        for (const auto hit : track.recHits()) {
          if (hit->isValid() == false)
            continue;
          MTDDetId Hit = hit->geographicalId();
          if ((Hit.det() == 6) && (Hit.subdetId() == 1) && (Hit.mtdSubDetector() == 1)) {
            MTDBtl = true;
            numMTDBtlvalidhits++;
            const auto* mtdhit = static_cast<const MTDTrackingRecHit*>(hit);
            const auto& hitCluster = mtdhit->mtdCluster();
            if (hitCluster.size() != 0) {
              auto recoClusterRef = edmNew::makeRefTo(btlRecCluHandle, &hitCluster);
              recoClustersRefs.push_back(recoClusterRef);
            }
          }
        }
        // --- keeping only tracks with last hit in MTD ---
        if (MTDBtl == true) {
          isBTL = true;
        }
      }  //loop over (geometrical) BTL tracks

      else {
        // --- all ETL tracks (with and without hit in MTD) ---
        int numMTDEtlvalidhits = 0;
        for (const auto hit : track.recHits()) {
          if (hit->isValid() == false)
            continue;
          MTDDetId Hit = hit->geographicalId();
          if ((Hit.det() == 6) && (Hit.subdetId() == 1) && (Hit.mtdSubDetector() == 2)) {
            isETL = true;
            ETLDetId ETLHit = hit->geographicalId();

            const auto* mtdhit = static_cast<const MTDTrackingRecHit*>(hit);
            const auto& hitCluster = mtdhit->mtdCluster();
            if (hitCluster.size() != 0) {
              auto recoClusterRef = edmNew::makeRefTo(etlRecCluHandle, &hitCluster);
              recoClustersRefs.push_back(recoClusterRef);
            }

            if ((ETLHit.zside() == -1) && (ETLHit.nDisc() == 1)) {
              MTDEtlZnegD1 = true;
              numMTDEtlvalidhits++;
            }
            if ((ETLHit.zside() == -1) && (ETLHit.nDisc() == 2)) {
              MTDEtlZnegD2 = true;
              numMTDEtlvalidhits++;
            }
            if ((ETLHit.zside() == 1) && (ETLHit.nDisc() == 1)) {
              MTDEtlZposD1 = true;
              numMTDEtlvalidhits++;
            }
            if ((ETLHit.zside() == 1) && (ETLHit.nDisc() == 2)) {
              MTDEtlZposD2 = true;
              numMTDEtlvalidhits++;
            }
          }
        }

        // --- keeping only tracks with last hit in MTD ---
        ETLdisc1 = (MTDEtlZnegD1 || MTDEtlZposD1);
        ETLdisc2 = (MTDEtlZnegD2 || MTDEtlZposD2);
        twoETLdiscs =
            ((MTDEtlZnegD1 == true) && (MTDEtlZnegD2 == true)) || ((MTDEtlZposD1 == true) && (MTDEtlZposD2 == true));
        // if (ETLdisc1 || ETLdisc2) {
        //   meETLTrackEtaMtd_->Fill(std::abs(trackGen.eta()));
        //   meETLTrackPhiMtd_->Fill(trackGen.phi());
        //   meETLTrackPtMtd_->Fill(trackGen.pt());
        //   if (twoETLdiscs) {
        //     meETLTrackEta2Mtd_->Fill(std::abs(trackGen.eta()));
        //     meETLTrackPhi2Mtd_->Fill(trackGen.phi());
        //     meETLTrackPt2Mtd_->Fill(trackGen.pt());
        //   }
        // }
      }

      // if (isBTL)
      //   meTrackOutermostHitR_->Fill(outermostHitPosition[trackref]);
      // if (isETL)
      //   meTrackOutermostHitZ_->Fill(std::abs(outermostHitPosition[trackref]));

      LogDebug("MVATrainingNtuple") << "Track p/pt = " << trackGen.p() << " " << trackGen.pt() << " eta "
                                      << trackGen.eta() << " BTL " << isBTL << " ETL " << isETL << " 2disks "
                                      << twoETLdiscs;

      // == TrackingParticle based matching
      const reco::TrackBaseRef trkrefb(trackref);
      auto tp_info = getMatchedTP(trkrefb);
      if (tp_info != nullptr && trkTPSelAll(**tp_info)) {

        // bool hasTime = false;
        // double tsim = (*tp_info)->parentVertex()->position().t() * simUnit_;
        // double dT(-9999.);
        // double pullT(-9999.);
        // if (sigmat0Safe[trackref] != -1.) {
        //   dT = t0Safe[trackref] - tsim;
        //   pullT = dT / sigmat0Safe[trackref];
        //   hasTime = true;
        // }

        // ==  MC truth matching

        bool isTPmtdDirectBTL = false, isTPmtdOtherBTL = false, isTPmtdDirectCorrectBTL = false,
             isTPmtdOtherCorrectBTL = false, isTPmtdETLD1 = false, isTPmtdETLD2 = false, isTPmtdCorrectETLD1 = false,
             isTPmtdCorrectETLD2 = false;
        // bool isFromSameTP = false;

        auto simClustersRefsIt = tp2SimAssociationMap.find(*tp_info);
        const bool withMTD = (simClustersRefsIt != tp2SimAssociationMap.end());

        // If there is a mtdSimLayerCluster from the tracking particle
        if (withMTD) {
          // -- Get the refs to MtdSimLayerClusters associated to the TP
          std::vector<edm::Ref<MtdSimLayerClusterCollection>> simClustersRefs;
          for (const auto& ref : simClustersRefsIt->val) {
            simClustersRefs.push_back(ref);
            MTDDetId mtddetid = ref->detIds_and_rows().front().first;
            if (mtddetid.mtdSubDetector() == 2) {
              ETLDetId detid(mtddetid.rawId());
              if (detid.nDisc() == 1)
                isTPmtdETLD1 = true;
              if (detid.nDisc() == 2)
                isTPmtdETLD2 = true;
            }
          }
          // === BTL
          // -- Sort BTL sim clusters by time
          std::vector<edm::Ref<MtdSimLayerClusterCollection>>::iterator directSimClusIt;
          if (std::abs(trackGen.eta()) < trackMaxBtlEta_ && !simClustersRefs.empty()) {
            std::sort(simClustersRefs.begin(), simClustersRefs.end(), [](const auto& a, const auto& b) {
              return a->simLCTime() < b->simLCTime();
            });
            // Find the first direct hit in time
            directSimClusIt = std::find_if(simClustersRefs.begin(), simClustersRefs.end(), [](const auto& simCluster) {
              MTDDetId mtddetid = simCluster->detIds_and_rows().front().first;
              return (mtddetid.mtdSubDetector() == 1 && simCluster->trackIdOffset() == 0);
            });
            // Check if TP has direct or other sim cluster for BTL
            for (const auto& simClusterRef : simClustersRefs) {
              if (directSimClusIt != simClustersRefs.end() && simClusterRef == *directSimClusIt) {
                isTPmtdDirectBTL = true;
              } else if (simClusterRef->trackIdOffset() != 0) {
                isTPmtdOtherBTL = true;
              }
            }
          }

          // ==  Check if the track-cluster association is correct: Track->RecoClus->SimClus == Track->TP->SimClus
          for (const auto& recClusterRef : recoClustersRefs) {
            if (recClusterRef.isNonnull()) {
              auto itp = r2sAssociationMap.equal_range(recClusterRef);
              if (itp.first != itp.second) {
                auto& simClustersRefs_RecoMatch = (*itp.first).second;

                BTLDetId RecoDetId((*recClusterRef).id());

                for (const auto& simClusterRef_RecoMatch : simClustersRefs_RecoMatch) {
                  // Check if simClusterRef_RecoMatch  exists in SimClusters
                  auto simClusterIt =
                      std::find(simClustersRefs.begin(), simClustersRefs.end(), simClusterRef_RecoMatch);
                  // SimCluster found in SimClusters
                  if (simClusterIt != simClustersRefs.end()) {
                    // isFromSameTP = true;
                    if (isBTL) {
                      if (directSimClusIt != simClustersRefs.end() && simClusterRef_RecoMatch == *directSimClusIt) {
                        isTPmtdDirectCorrectBTL = true;
                      } else if (simClusterRef_RecoMatch->trackIdOffset() != 0) {
                        isTPmtdOtherCorrectBTL = true;
                      }
                    }
                    if (isETL) {
                      MTDDetId mtddetid = (*simClusterIt)->detIds_and_rows().front().first;
                      ETLDetId detid(mtddetid.rawId());
                      if (detid.nDisc() == 1)
                        isTPmtdCorrectETLD1 = true;
                      if (detid.nDisc() == 2)
                        isTPmtdCorrectETLD2 = true;
                    }
                  }
                }
              }
            }
          }  /// end loop over reco clusters associated to this track.

        }  // --- end "withMTD"

        Ttrack_pt.push_back(trackGen.pt());
        Ttrack_phi.push_back(trackGen.phi());
        Ttrack_eta.push_back(trackGen.eta());
        Ttrack_dz.push_back(std::abs(trackGen.dz(beamSpot.position())));
        Ttrack_dxy.push_back(std::abs(trackGen.dxy(beamSpot.position())));
        Ttrack_chi2.push_back(trackGen.chi2());
        Ttrack_ndof.push_back(trackGen.ndof());
        Ttrack_nValidHits.push_back(trackGen.numberOfValidHits());

        Ttrack_npixBarrelValidHits.push_back(npixBarrel[trackref]);
        Ttrack_npixEndcapValidHits.push_back(npixEndcap[trackref]);
        Ttrack_BTLchi2.push_back(btlMatchChi2[trackref]);
        Ttrack_BTLtime_chi2.push_back(btlMatchTimeChi2[trackref]);
        Ttrack_ETLchi2.push_back(etlMatchChi2[trackref]);
        Ttrack_ETLtime_chi2.push_back(etlMatchTimeChi2[trackref]);

        Ttrack_t0.push_back(t0Src[trackref]);
        Ttrack_sigmat0.push_back(sigmat0Src[trackref]);
        Ttrack_Tmtd.push_back(tMtd[trackref]);
        Ttrack_sigmaTmtd.push_back(sigmatimemtd[trackref]);
        Ttrack_length.push_back(pathLength[trackref]);
        Ttrack_MtdMVA.push_back(mtdQualMVA[trackref]);
        Ttrack_lHitPos.push_back(outermostHitPosition[trackref]);

        Ttrack_isBTL.push_back(isBTL);
        Ttrack_isETL.push_back(isETL);
        Ttrack_ETLdisc1.push_back(ETLdisc1);
        Ttrack_ETLdisc2.push_back(ETLdisc2);

        if ((*tp_info)->eventId().bunchCrossing() == 0 &&
            (*tp_info)->eventId().event() == 0) {  // Signal vs PU seperation
          Ttrack_Signal.push_back(true);         // Signal track
        } else {
          Ttrack_Signal.push_back(false);  // PU track?
        }

        if (isTPmtdDirectCorrectBTL || isTPmtdOtherCorrectBTL ||
            isTPmtdCorrectETLD1 || isTPmtdCorrectETLD2) {
          Ttrack_Associated.push_back(true);  // Associated track
        } else {
          Ttrack_Associated.push_back(false);  // Not associated track
        }
        if (isTPmtdDirectBTL || isTPmtdETLD1 || isTPmtdETLD2) {
          isDirClu = true;  // Direct cluster
          isOtherClu = false;
        }
        if (isTPmtdOtherBTL) {
          isDirClu = false;  // Not direct cluster
          isOtherClu = true;
        }

        Ttrack_DirClu.push_back(isDirClu);  // Not direct cluster
        Ttrack_OtherClu.push_back(isOtherClu);
      }  // TP matching
    }  // trkRecSel

  }  // RECO tracks loop

  // std::cout << "Number of /*selected tracks in event layer 1: " << iEvent.id().event() << ": " << nSelectedTracks1 << std::endl;
  // std::cout << "Number of selected tracks in event layer 2: " << iEvent.id().event() << ": " << nSelectedTracks2 << std::endl;
  // std::cout << "Number of */selected tracks in event layer 3: " << iEvent.id().event() << ": " << nSelectedTracks3 << std::endl;
  BDTtree->Fill();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------

void MVATrainingNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  // RecVertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("inputTagV"));
  // RecBeamSpotToken_ = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("offlineBS"));
  // trackingVertexCollectionToken_ = consumes<TrackingVertexCollection>(iConfig.getParameter<edm::InputTag>("SimTag"));
  // sigmatimeToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmaSrc"));
  // tofPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofPi"));
  // tofKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofK"));
  // tofPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("tofP"));
  // probPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probPi"));
  // probKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probK"));
  // probPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("probP"));
  // sigmaTofPiToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofpiSrc"));
  // sigmaTofKToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofkSrc"));
  // sigmaTofPToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("sigmatofpSrc"));
  // btlMatchChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("btlMatchChi2Src"));
  // etlMatchChi2Token_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("etlMatchChi2Src"));
  // npixBarrelToken_ = consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("npixBarrelSrc"));
  // npixEndcapToken_ = consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("npixEndcapSrc"));

  desc.add<edm::InputTag>("offlineBS", edm::InputTag("offlineBeamSpot"));
  desc.add<edm::InputTag>("tofPi", edm::InputTag("trackExtenderWithMTD:generalTrackTofPi"));
  desc.add<edm::InputTag>("tofK", edm::InputTag("trackExtenderWithMTD:generalTrackTofK"));
  desc.add<edm::InputTag>("tofP", edm::InputTag("trackExtenderWithMTD:generalTrackTofP"));
  desc.add<edm::InputTag>("probPi", edm::InputTag("tofPID:probPi"));
  desc.add<edm::InputTag>("probK", edm::InputTag("tofPID:probK"));
  desc.add<edm::InputTag>("probP", edm::InputTag("tofPID:probP"));
  desc.add<edm::InputTag>("momentumSrc", edm::InputTag("trackExtenderWithMTD:generalTrackp"));
  desc.add<edm::InputTag>("sigmatofpiSrc", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofPi"));
  desc.add<edm::InputTag>("sigmatofkSrc", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofK"));
  desc.add<edm::InputTag>("sigmatofpSrc", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofP"));
  desc.add<edm::InputTag>("btlMatchChi2Src", edm::InputTag("trackExtenderWithMTD", "btlMatchChi2"));
  desc.add<edm::InputTag>("etlMatchChi2Src", edm::InputTag("trackExtenderWithMTD", "etlMatchChi2"));
  desc.add<edm::InputTag>("npixBarrelSrc", edm::InputTag("trackExtenderWithMTD", "npixBarrel"));
  desc.add<edm::InputTag>("npixEndcapSrc", edm::InputTag("trackExtenderWithMTD", "npixEndcap"));

  desc.add<std::string>("folder", "MTD/Tracks");
  desc.add<edm::InputTag>("inputTagG", edm::InputTag("generalTracks"));
  desc.add<edm::InputTag>("inputTracks", edm::InputTag("generalTracks"));
  desc.add<edm::InputTag>("inputTagT", edm::InputTag("trackExtenderWithMTD"));
  desc.add<edm::InputTag>("inputTagV", edm::InputTag("offlinePrimaryVertices4D"));
  // desc.add<edm::InputTag>("inputTagH", edm::InputTag("generatorSmeared"));
  desc.add<edm::InputTag>("SimTag", edm::InputTag("mix", "MergedTrackTruth"));
  desc.add<edm::InputTag>("TPtoRecoTrackAssoc", edm::InputTag("trackingParticleRecoTrackAsssociation"));
  desc.add<edm::InputTag>("tp2SimAssociationMapTag", edm::InputTag("mtdSimLayerClusterToTPAssociation"));
  desc.add<edm::InputTag>("Sim2tpAssociationMapTag", edm::InputTag("mtdSimLayerClusterToTPAssociation"));
  desc.add<edm::InputTag>("r2sAssociationMapTag", edm::InputTag("mtdRecoClusterToSimLayerClusterAssociation"));
  desc.add<edm::InputTag>("btlRecHits", edm::InputTag("mtdRecHits", "FTLBarrel"));
  desc.add<edm::InputTag>("etlRecHits", edm::InputTag("mtdRecHits", "FTLEndcap"));
  desc.add<edm::InputTag>("recCluTagBTL", edm::InputTag("mtdClusters", "FTLBarrel"));
  desc.add<edm::InputTag>("recCluTagETL", edm::InputTag("mtdClusters", "FTLEndcap"));
  desc.add<edm::InputTag>("tmtd", edm::InputTag("trackExtenderWithMTD:generalTracktmtd"));
  // desc.add<edm::InputTag>("sigmatmtd", edm::InputTag("trackExtenderWithMTD:generalTracksigmatmtd"));
  desc.add<edm::InputTag>("sigmaSrc", edm::InputTag("trackExtenderWithMTD:generalTracksigmatmtd"));
  desc.add<edm::InputTag>("t0Src", edm::InputTag("trackExtenderWithMTD:generalTrackt0"));
  desc.add<edm::InputTag>("sigmat0Src", edm::InputTag("trackExtenderWithMTD:generalTracksigmat0"));
  desc.add<edm::InputTag>("trackAssocSrc", edm::InputTag("trackExtenderWithMTD:generalTrackassoc"))
      ->setComment("Association between General and MTD Extended tracks");
  desc.add<edm::InputTag>("pathLengthSrc", edm::InputTag("trackExtenderWithMTD:generalTrackPathLength"));
  desc.add<edm::InputTag>("btlMatchTimeChi2", edm::InputTag("trackExtenderWithMTD:btlMatchTimeChi2"));
  desc.add<edm::InputTag>("etlMatchTimeChi2", edm::InputTag("trackExtenderWithMTD:etlMatchTimeChi2"));
  desc.add<edm::InputTag>("btlMatchChi2", edm::InputTag("trackExtenderWithMTD:btlMatchChi2"));
  desc.add<edm::InputTag>("t0SafePID", edm::InputTag("tofPID:t0safe"));
  desc.add<edm::InputTag>("sigmat0SafePID", edm::InputTag("tofPID:sigmat0safe"));
  desc.add<edm::InputTag>("sigmat0PID", edm::InputTag("tofPID:sigmat0"));
  desc.add<edm::InputTag>("t0PID", edm::InputTag("tofPID:t0"));
  desc.add<edm::InputTag>("sigmaTofPi", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofPi"));
  desc.add<edm::InputTag>("sigmaTofK", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofK"));
  desc.add<edm::InputTag>("sigmaTofP", edm::InputTag("trackExtenderWithMTD:generalTrackSigmaTofP"));
  desc.add<edm::InputTag>("trackMVAQual", edm::InputTag("mtdTrackQualityMVA:mtdQualMVA"));
  desc.add<edm::InputTag>("outermostHitPositionSrc",
                          edm::InputTag("trackExtenderWithMTD:generalTrackOutermostHitPosition"));
  desc.addUntracked<std::string>("fileName", "file.root");

  // desc.add<double>("trackMaximumPt", 12.);  // [GeV]
  desc.add<double>("trackMaximumBtlEta", 1.5);
  desc.add<double>("trackMinimumEtlEta", 1.6);
  desc.add<double>("trackMaximumEtlEta", 3.);

  {
    edm::ParameterSetDescription psd0;
    HITrackFilterForPVFinding::fillPSetDescription(psd0);  // extension of TrackFilterForPVFinding
    desc.add<edm::ParameterSetDescription>("TkFilterParameters", psd0);
  }

  descriptions.add("mvaTrainingNtuple", desc);
}

// const bool MVATrainingNtuple::trkTPSelLV(const TrackingParticle& tp) {
//   bool match = (tp.status() != 1) ? false : true;
//   return match;
// }

const bool MVATrainingNtuple::trkTPSelAll(const TrackingParticle& tp) {
  bool match = false;

  auto x_pv = tp.parentVertex()->position().x();
  auto y_pv = tp.parentVertex()->position().y();
  auto z_pv = tp.parentVertex()->position().z();

  auto r_pv = std::sqrt(x_pv * x_pv + y_pv * y_pv);

  match = tp.charge() != 0 && std::abs(tp.eta()) < etacutGEN_ && tp.pt() > pTcutBTL_ && r_pv < rBTL_ &&
          std::abs(z_pv) < zETL_;
  return match;
}

const bool MVATrainingNtuple::trkRecSel(const reco::TrackBase& trk) {
  bool match = false;
  match = std::abs(trk.eta()) <= etacutREC_ && trk.pt() > pTcutBTL_;
  return match;
}

// const bool MVATrainingNtuple::trkRecSelLowPt(const reco::TrackBase& trk) {
//   bool match = false;
//   match = std::abs(trk.eta()) <= etacutREC_ && trk.pt() > pTcutETL_ && trk.pt() < pTcutBTL_;
//   return match;
// }

const edm::Ref<std::vector<TrackingParticle>>* MVATrainingNtuple::getMatchedTP(const reco::TrackBaseRef& recoTrack) {
  auto found = r2s_->find(recoTrack);

  // reco track not matched to any TP
  if (found == r2s_->end())
    return nullptr;

  //matched TP equal to any TP associated to in time events
  for (const auto& tp : found->val) {
    if (tp.first->eventId().bunchCrossing() == 0)
      return &tp.first;
  }

  // reco track not matched to any TP from vertex
  return nullptr;
}

//define this as a plug-in
DEFINE_FWK_MODULE(MVATrainingNtuple);
