#ifndef DataFormats_L1TParticleFlow_datatypes_h
#define DataFormats_L1TParticleFlow_datatypes_h

#include <ap_int.h>
#include <cassert>
#include <cmath>

namespace l1ct {

  typedef ap_ufixed<14, 12, AP_TRN, AP_SAT> pt_t;
  typedef ap_ufixed<17, 15, AP_TRN, AP_SAT> mass2_t;  // can store up to 256 GeV with 0.5 GeV precision
  typedef ap_ufixed<10, 8, AP_TRN, AP_SAT> pt10_t;
  typedef ap_fixed<16, 14, AP_TRN, AP_SAT> dpt_t;
  typedef ap_ufixed<28, 24, AP_TRN, AP_SAT> pt2_t;
  typedef ap_int<10> eta_t;
  typedef ap_int<10> phi_t;
  typedef ap_int<6> tkdeta_t;
  typedef ap_uint<7> tkdphi_t;
  typedef ap_int<12> glbeta_t;
  typedef ap_int<11> glbphi_t;
  typedef ap_int<5> vtx_t;
  typedef ap_int<10> z0_t;         // 40cm / 0.1
  typedef ap_uint<8> dxy_t;        // sqrt(32)cm / 0.004 packed into 8 bit for l1ct
  typedef ap_uint<3> tkquality_t;  // tbd
  typedef ap_ufixed<9, 1, AP_RND_CONV, AP_WRAP> puppiWgt_t;
  typedef ap_uint<6> emid_t;
  typedef ap_uint<14> tk2em_dr_t;
  typedef ap_uint<14> tk2calo_dr_t;
  typedef ap_uint<10> em2calo_dr_t;
  typedef ap_uint<13> tk2calo_dq_t;
  typedef ap_uint<4> egquality_t;
  typedef ap_uint<3> stub_t;
  typedef ap_ufixed<10, 1, AP_TRN, AP_SAT> srrtot_t;
  typedef ap_uint<8> meanz_t;  // mean - MEANZ_OFFSET(= 320 cm)
  typedef ap_ufixed<10, 5, AP_TRN, AP_SAT> hoe_t;
  typedef ap_uint<4> redChi2Bin_t;
  typedef ap_fixed<10, 1, AP_RND_CONV, AP_SAT> id_score_t;  // ID score to be between -1 (background) and 1 (signal)
  typedef ap_ufixed<8, 1, AP_RND_CONV, AP_SAT> jet_tag_score_t;  // 8 bit jet jet probability from 0 to 1
  //FIXME: no optimization for now
  typedef ap_ufixed<8, 1, AP_RND_CONV, AP_SAT> id_prob_t;  // ID prob between 0 and 1 (signal)

  // FIXME: adjust range 10-11bits -> 1/4 - 1/2TeV is probably more than enough for all reasonable use cases
  typedef ap_ufixed<11, 9, AP_TRN, AP_SAT> iso_t;
  typedef ap_ufixed<6, 0, AP_RND, AP_SAT> rel_iso_t;
  typedef ap_ufixed<6, 0, AP_RND, AP_SAT> shower_shape_t;

  struct ParticleID {
    ap_uint<3> bits;
    enum PID {
      NONE = 0,
      HADZERO = 0,
      PHOTON = 1,
      HADMINUS = 2,
      HADPLUS = 3,
      ELEMINUS = 4,
      ELEPLUS = 5,
      MUMINUS = 6,
      MUPLUS = 7
    };
    enum PTYPE { HAD = 0, EM = 1, MU = 2 };

    ParticleID(PID val = NONE) : bits(val) {}
    ParticleID &operator=(PID val) {
      bits = val;
      return *this;
    }

    int rawId() const { return bits.to_int(); }
    bool isPhoton() const {
#ifndef __SYNTHESIS__
      assert(neutral());
#endif
      return bits[0];
    }
    bool isMuon() const { return bits[2] && bits[1]; }
    bool isElectron() const { return bits[2] && !bits[1]; }
    bool isChargedHadron() const { return !bits[2] && bits[1]; }
    bool charge() const {
#ifndef __SYNTHESIS__
      assert(charged());
#endif
      return bits[0]; /* 1 if positive, 0 if negative */
    }

    bool chargeOrNull() const {  // doesn't throw on null id
      return bits[0];
    }
    bool charged() const { return bits[1] || bits[2]; };
    bool neutral() const { return !charged(); }
    int intCharge() const { return charged() ? (charge() ? +1 : -1) : 0; }

    void clear() { bits = 0; }

    static ParticleID mkChHad(bool charge) { return ParticleID(charge ? HADPLUS : HADMINUS); }
    static ParticleID mkElectron(bool charge) { return ParticleID(charge ? ELEPLUS : ELEMINUS); }
    static ParticleID mkMuon(bool charge) { return ParticleID(charge ? MUPLUS : MUMINUS); }

    inline bool operator==(const ParticleID &other) const { return bits == other.bits; }

    inline int pdgId() const {
      switch (bits.to_int()) {
        case HADZERO:
          return 130;
        case PHOTON:
          return 22;
        case HADMINUS:
          return -211;
        case HADPLUS:
          return +211;
        case ELEMINUS:
          return +11;
        case ELEPLUS:
          return -11;
        case MUMINUS:
          return +13;
        case MUPLUS:
          return -13;
      }
      return 0;
    }

    inline int oldId() const {
      //{ PID_Charged=0, PID_Neutral=1, PID_Photon=2, PID_Electron=3, PID_Muon=4 };
      switch (bits.to_int()) {
        case HADZERO:
          return 1;
        case PHOTON:
          return 2;
        case HADMINUS:
          return 0;
        case HADPLUS:
          return 0;
        case ELEMINUS:
          return 3;
        case ELEPLUS:
          return 3;
        case MUMINUS:
          return 4;
        case MUPLUS:
          return 4;
      }
      return -1;
    }
  };

  namespace Scales {

    constexpr int INTPHI_PI = 720;
    constexpr int INTPHI_TWOPI = 2 * INTPHI_PI;
    constexpr float INTPT_LSB = 0.25;
    constexpr float ETAPHI_LSB = M_PI / INTPHI_PI;
    constexpr float Z0_LSB = 0.05;
    constexpr float DXY_LSB = 0.00390625;    // -16 to 16 / 2**13 from track word
    constexpr float DXYSQRT_LSB = 0.015625;  //sqrt(abs(dxy)) 4/2**8 (8 bit for l1ct dxy)
    constexpr float PUPPIW_LSB = 1.0 / 256;
    constexpr float MEANZ_OFFSET = 320.;
    constexpr float SRRTOT_LSB = 0.0019531250;  // pow(2, -9)
    constexpr unsigned int SRRTOT_SCALE = 64;   // pow(2, 6)
    constexpr float HOE_LSB = 0.031250000;      // pow(2, -5)
    // empirical choice: we saturate to 16 while waiting for updates to the GCT-CTL1 interface document
    constexpr unsigned int RELISO_SCALE = 16;

    inline float floatPt(pt_t pt) { return pt.to_float(); }
    inline float floatPt(dpt_t pt) { return pt.to_float(); }
    inline float floatPt(pt2_t pt2) { return pt2.to_float(); }
    inline int intPt(pt_t pt) {
      ap_uint<pt_t::width> rawPt = pt.range();
      return rawPt.to_int();
    }
    inline int intPt(dpt_t pt) {
      ap_int<dpt_t::width> rawPt = pt.range();
      return rawPt.to_int();
    }
    inline float floatEta(eta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(phi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatEta(tkdeta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(tkdphi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatEta(glbeta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(glbphi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatZ0(z0_t z0) { return z0.to_float() * Z0_LSB; }
    inline float floatDxy(dxy_t dxy) { return dxy.to_float() * DXYSQRT_LSB; }  // l1ct stores sqrt(dxy)
    inline float floatPuppiW(puppiWgt_t puppiw) { return puppiw.to_float(); }
    inline float floatIso(iso_t iso) { return iso.to_float(); }
    inline float floatSrrTot(srrtot_t srrtot) { return srrtot.to_float() / SRRTOT_SCALE; };
    inline float floatMeanZ(meanz_t meanz) { return meanz + MEANZ_OFFSET; };
    inline float floatHoe(hoe_t hoe) { return hoe.to_float(); };
    inline float floatIDScore(id_score_t score) { return score.to_float(); };
    inline float floatMass(mass2_t mass) { return mass.to_float(); }
    inline float floatIDProb(id_prob_t prob) { return prob.to_float(); };
    inline float floatRelIso(rel_iso_t rel_iso) { return rel_iso.to_float() * RELISO_SCALE; }
    inline float floatShoweShape(shower_shape_t showe_shape) { return showe_shape.to_float(); }

    inline pt_t makePt(int pt) { return ap_ufixed<16, 14>(pt) >> 2; }
    inline dpt_t makeDPt(int dpt) { return ap_fixed<18, 16>(dpt) >> 2; }
    inline pt_t makePtFromFloat(float pt) { return pt_t(0.25 * std::round(pt * 4)); }
    inline dpt_t makeDPtFromFloat(float dpt) { return dpt_t(dpt); }
    inline z0_t makeZ0(float z0) { return z0_t(std::round(z0 / Z0_LSB)); }
    inline dxy_t makeDxy(float dxy) { return dxy_t(std::round(dxy / DXYSQRT_LSB)); }  // l1ct stores sqrt(dxy)

    inline ap_uint<pt_t::width> ptToInt(pt_t pt) {
      // note: this can be synthethized, e.g. when pT is used as intex in a LUT
      ap_uint<pt_t::width> ret = 0;
      ret(pt_t::width - 1, 0) = pt(pt_t::width - 1, 0);
      return ret;
    }

    inline ap_int<dpt_t::width> ptToInt(dpt_t pt) {
      // note: this can be synthethized, e.g. when pT is used as intex in a LUT
      ap_uint<dpt_t::width> ret = 0;
      ret(dpt_t::width - 1, 0) = pt(dpt_t::width - 1, 0);
      return ret;
    }

    inline phi_t makePhi(float phi) { return round(phi / ETAPHI_LSB); }
    inline eta_t makeEta(float eta) { return round(eta / ETAPHI_LSB); }
    inline glbeta_t makeGlbEta(float eta) { return round(eta / ETAPHI_LSB); }
    inline glbeta_t makeGlbEtaRoundEven(float eta) { return 2 * std::round(eta / ETAPHI_LSB / 2); }

    inline glbphi_t makeGlbPhi(float phi) { return round(phi / ETAPHI_LSB); }
    inline iso_t makeIso(float iso) { return iso_t(0.25 * round(iso * 4)); }

    inline int makeDR2FromFloatDR(float dr) { return ceil(dr * dr / ETAPHI_LSB / ETAPHI_LSB); }
    inline srrtot_t makeSrrTot(float var) { return srrtot_t(SRRTOT_LSB * round(var * SRRTOT_SCALE / SRRTOT_LSB)); };
    inline meanz_t makeMeanZ(float var) { return round(var - MEANZ_OFFSET); };
    inline hoe_t makeHoe(float var) { return hoe_t(HOE_LSB * round(var / HOE_LSB)); };
    inline rel_iso_t makeRelIso(float var) { return rel_iso_t(var / RELISO_SCALE); };

    inline float maxAbsEta() { return ((1 << (eta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsPhi() { return ((1 << (phi_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbEta() { return ((1 << (glbeta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbPhi() { return ((1 << (glbphi_t::width - 1)) - 1) * ETAPHI_LSB; }
  }  // namespace Scales

  inline int dr2_int(eta_t eta1, phi_t phi1, eta_t eta2, phi_t phi2) {
    ap_int<eta_t::width + 1> deta = (eta1 - eta2);
    ap_int<phi_t::width + 1> dphi = (phi1 - phi2);
    return deta * deta + dphi * dphi;
  }

}  // namespace l1ct

#endif
