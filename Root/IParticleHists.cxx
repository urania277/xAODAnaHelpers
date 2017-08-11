#include <xAODAnaHelpers/IParticleHists.h>
#include <sstream>

namespace util {
  template <typename T> std::string to_string(const T& t) {
    std::string str{std::to_string (t)};
    int offset{1};
    if (str.find_last_not_of('0') == str.find('.')) { offset = 0; }
    str.erase(str.find_last_not_of('0') + offset, std::string::npos);
    return str;
  }
}

using std::vector;

IParticleHists :: IParticleHists (std::string name, std::string detailStr, std::string prefix, std::string title) :
  HistogramManager(name, detailStr),
  m_infoSwitch(new HelperClasses::IParticleInfoSwitch(m_detailStr)),
  m_prefix(prefix),m_title(title)
{
  m_debug = false;
}

IParticleHists :: ~IParticleHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

void trimString(std::string &s) {
  int offset = 1;
  if (s.find_last_not_of('0') == s.find('.'))
    offset = 0;
  s.erase(s.find_last_not_of('0') + offset, std::string::npos);
}

StatusCode IParticleHists::initialize() {

  // These plots are always made
  m_Pt_l        = book(m_name, m_prefix+"Pt_l",     m_title+" p_{T} [GeV]", 120, 0, 3000.);
  m_Pt          = book(m_name, m_prefix+"Pt",       m_title+" p_{T} [GeV]", 100, 0, 1000.);
  m_Pt_m        = book(m_name, m_prefix+"Pt_m",     m_title+" p_{T} [GeV]", 100, 0,  500.);
  m_Pt_s        = book(m_name, m_prefix+"Pt_s",     m_title+" p_{T} [GeV]", 100, 0,  100.);
  m_Eta         = book(m_name, m_prefix+"Eta",      m_title+" #eta",         80, -4, 4);
  m_Phi         = book(m_name, m_prefix+"Phi",      m_title+" Phi",         120, -TMath::Pi(), TMath::Pi() );
  m_M           = book(m_name, m_prefix+"Mass",     m_title+" Mass [GeV]",  120, 0, 400);
  m_E           = book(m_name, m_prefix+"Energy",   m_title+" Energy [GeV]",120, 0, 4000.);
  m_Rapidity    = book(m_name, m_prefix+"Rapidity", m_title+" Rapidity",    120, -10, 10);

  if(m_debug) Info("IParticleHists::initialize()", m_name.c_str());
  // details of the particle kinematics
  if( m_infoSwitch->m_kinematic ) {
    if(m_debug) Info("IParticleHists::initialize()", "adding kinematic plots");
    m_Px     = book(m_name, m_prefix+"Px",     m_title+" Px [GeV]",     120, 0, 1000);
    m_Py     = book(m_name, m_prefix+"Py",     m_title+" Py [GeV]",     120, 0, 1000);
    m_Pz     = book(m_name, m_prefix+"Pz",     m_title+" Pz [GeV]",     120, 0, 4000);

    m_Et          = book(m_name, m_prefix+"Et",       m_title+" E_{T} [GeV]", 100, 0, 1000.);
    m_Et_m        = book(m_name, m_prefix+"Et_m",     m_title+" E_{T} [GeV]", 100, 0,  500.);
    m_Et_s        = book(m_name, m_prefix+"Et_s",     m_title+" E_{T} [GeV]", 100, 0,  100.);
  }

  // N leading jets
  if( m_infoSwitch->m_numLeading > 0 ){
    std::stringstream pNum;
    std::stringstream pTitle;
    for(int iParticle=0; iParticle < m_infoSwitch->m_numLeading; ++iParticle){
      pNum << iParticle;
      
      pTitle << iParticle+1;
      switch(iParticle)
	{
	case 0:
	  pTitle << "^{st}";
	  break;
	case 1:
	  pTitle << "^{nd}";
	  break;
	case 2:
	  pTitle << "^{rd}";
	  break;
	default:
	  pTitle << "^{th}";
	  break;
	}
      
      m_NPt_l.push_back(       book(m_name, (m_prefix+"Pt_l_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,120,            0,       3000. ) );
      m_NPt .push_back(       book(m_name, (m_prefix+"Pt_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       1000. ) );
      m_NPt_m.push_back(       book(m_name, (m_prefix+"Pt_m_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       500. ) );
      m_NPt_s.push_back(       book(m_name, (m_prefix+"Pt_s_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       100. ) );
      m_NEta.push_back(      book(m_name, (m_prefix+"Eta_"+pNum.str()),      pTitle.str()+" "+m_title+" #eta"        , 80,           -4,           4 ) );
      m_NPhi.push_back(      book(m_name, (m_prefix+"Phi_"+pNum.str()),      pTitle.str()+" "+m_title+" Phi"         ,120, -TMath::Pi(), TMath::Pi() ) );
      m_NM.push_back(        book(m_name, (m_prefix+"Mass_"+pNum.str()),     pTitle.str()+" "+m_title+" Mass [GeV]"  ,120,            0,         400 ) );
      m_NE.push_back(        book(m_name, (m_prefix+"Energy_"+pNum.str()),   pTitle.str()+" "+m_title+" Energy [GeV]",120,            0,       4000. ) );
      m_NRapidity.push_back( book(m_name, (m_prefix+"Rapidity_"+pNum.str()), pTitle.str()+" "+m_title+" Rapidity"    ,120,          -10,          10 ) );
      if(m_infoSwitch->m_kinematic){
	m_NEt .push_back(        book(m_name, (m_prefix+"Et_"+pNum.str()),         pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       1000. ) );
	m_NEt_m.push_back(       book(m_name, (m_prefix+"Et_m_"+pNum.str()),       pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       500. ) );
	m_NEt_s.push_back(       book(m_name, (m_prefix+"Et_s_"+pNum.str()),       pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       100. ) );
      }
      
      if(m_infoSwitch->m_TLA) {
        m_NPt_f.push_back(       book(m_name, "TLA_noExtraSel/Pt_f_"+pNum.str(),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,3000,            0,       3000. ) );
      }
      
      pNum.str("");
      pTitle.str("");

    }//for iParticle
  }


  if( m_infoSwitch->m_TLA ) {
    m_mjj        = book(m_name, "TLA_noExtraSel/mjj", "m_{jj} [GeV]", 5000, 0, 5000);
    m_yStar      = book(m_name, "TLA_noExtraSel/yStar", "yStar", 120, -3, 3);

    // book histograms for all the TLA regions
    for(auto leadPt : m_infoSwitch->m_TLA_leadPts) {
      for(auto subleadPt : m_infoSwitch->m_TLA_subleadPts) {
        for(auto mjj : m_infoSwitch->m_TLA_mjjs) {
          for(auto yStar : m_infoSwitch->m_TLA_yStars) {
            std::string regionName = "TLA_pTlead" + util::to_string(leadPt) + 
              "_pTsublead" + util::to_string(subleadPt) +
              "_mjj" + util::to_string(mjj) +
              "_yStar" + util::to_string(yStar);
            m_regions_mjj.push_back( book(m_name, regionName+"/mjj", "m_{jj} [GeV]", 5000, 0, 5000) );
            m_regions_Pt_lead.push_back( book(m_name, regionName+"/pTlead", "1^{st} jet p_{T} [GeV]", 3000, 0, 3000) );
            m_regions_Pt_sublead.push_back( book(m_name, regionName+"/pTsublead", "2^{nd} jet p_{T} [GeV]", 3000, 0, 3000) );
            m_regions_yStar.push_back( book(m_name, regionName+"/yStar", "yStar", 120, -3, 3) );
          }
          for(auto yStarAnti : m_infoSwitch->m_TLA_yStarAntis) {
            std::string regionName = "TLA_pTlead" + util::to_string(leadPt) + 
              "_pTsublead" + util::to_string(subleadPt) +
              "_mjj" + util::to_string(mjj) +
              "_yStarAnti" + util::to_string(yStarAnti);
            m_regions_mjj.push_back( book(m_name, regionName+"/mjj", "m_{jj} [GeV]", 5000, 0, 5000) );
            m_regions_Pt_lead.push_back( book(m_name, regionName+"/pTlead", "1^{st} jet p_{T} [GeV]", 3000, 0, 3000) );
            m_regions_Pt_sublead.push_back( book(m_name, regionName+"/pTsublead", "2^{nd} jet p_{T} [GeV]", 3000, 0, 3000) );
            m_regions_yStar.push_back( book(m_name, regionName+"/yStar", "yStar", 120, -3, 3) );
          }
        }
      }
    }

  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticleContainer* particles, float eventWeight, const xAOD::EventInfo* eventInfo) {
  for( auto particle_itr : *particles ) {
    RETURN_CHECK("IParticleHists::execute()", this->execute( particle_itr, eventWeight, eventInfo), "");
  }

  if( m_infoSwitch->m_numLeading > 0){
    int numParticles = std::min( m_infoSwitch->m_numLeading, (int)particles->size() );
    for(int iParticle=0; iParticle < numParticles; ++iParticle){
      m_NPt_l.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt_m.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt_s.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NEta.at(iParticle)->       Fill( particles->at(iParticle)->eta(),      eventWeight);
      m_NPhi.at(iParticle)->       Fill( particles->at(iParticle)->phi(),      eventWeight);
      m_NM.at(iParticle)->         Fill( particles->at(iParticle)->m()/1e3,    eventWeight);
      m_NE.at(iParticle)->         Fill( particles->at(iParticle)->e()/1e3,    eventWeight);
      m_NRapidity.at(iParticle)->  Fill( particles->at(iParticle)->rapidity(), eventWeight);

      if(m_infoSwitch->m_kinematic){
	float et = particles->at(iParticle)->e()/cosh(particles->at(iParticle)->eta())/1e3;
	m_NEt  .at(iParticle)->        Fill( et,   eventWeight);
	m_NEt_m.at(iParticle)->        Fill( et,   eventWeight);
	m_NEt_s.at(iParticle)->        Fill( et,   eventWeight);
      }

      // fine-binned pt hists
      if(m_infoSwitch->m_TLA){
        m_NPt_f.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      }
    }
  }

  // other TLA hists
  if(m_infoSwitch->m_TLA){
    // I am going to assume that there are two jets
    if( (int)particles->size() < 2 ) {
      std::cout << "WARNING    IParticleHists::execute()   Need at least two jets for TLA" << std::endl;
      return StatusCode::SUCCESS;
    }
    TLorentzVector jet0, jet1;
    jet0.SetPtEtaPhiE(particles->at(0)->pt()/1e3, particles->at(0)->eta(), particles->at(0)->phi(), particles->at(0)->e()/1e3);
    jet1.SetPtEtaPhiE(particles->at(1)->pt()/1e3, particles->at(1)->eta(), particles->at(1)->phi(), particles->at(1)->e()/1e3);

    double thismjj = (jet0 + jet1).M();
    double thisyStar = (jet0.Rapidity() - jet1.Rapidity())*0.5;
    m_mjj -> Fill (thismjj, eventWeight);
    m_yStar -> Fill (thisyStar, eventWeight);

    // fill TLA region hists
    unsigned long long eventNumber;
    bool printEventNumber = false;
    if(printEventNumber) {
      if(jet0.Pt() > 200 || jet0.Pt() < 195)
        printEventNumber = false;
    }
    if(printEventNumber)
      eventNumber = eventInfo->auxdata< unsigned long long >( "eventNumber" );
    int i = 0;
    for(auto leadPt : m_infoSwitch->m_TLA_leadPts) {
      for(auto subleadPt : m_infoSwitch->m_TLA_subleadPts) {
        for(auto mjj : m_infoSwitch->m_TLA_mjjs) {
          for(auto yStar : m_infoSwitch->m_TLA_yStars) {
            if( jet0.Pt() > leadPt &&
                jet1.Pt() > subleadPt &&
                thismjj > mjj &&
                fabs(thisyStar) < yStar ) {
              m_regions_mjj.at(i) -> Fill(thismjj, eventWeight);
              m_regions_yStar.at(i) -> Fill(thisyStar, eventWeight);
              m_regions_Pt_lead.at(i) -> Fill(jet0.Pt(), eventWeight);
              m_regions_Pt_sublead.at(i) -> Fill(jet1.Pt(), eventWeight);
              if(printEventNumber) std::cout << eventNumber << ": " << jet0.Pt() << " > " << leadPt << ", " << jet1.Pt() << " > " << subleadPt << ", fabs(" << thisyStar << ") < " << yStar << std::endl;
            }
            i++;
          }
          for(auto yStarAnti : m_infoSwitch->m_TLA_yStarAntis) {
            if( jet0.Pt() > leadPt &&
                jet1.Pt() > subleadPt &&
                thismjj > mjj &&
                fabs(thisyStar) > yStarAnti ) {
              m_regions_mjj.at(i) -> Fill(thismjj, eventWeight);
              m_regions_yStar.at(i) -> Fill(thisyStar, eventWeight);
              m_regions_Pt_lead.at(i) -> Fill(jet0.Pt(), eventWeight);
              m_regions_Pt_sublead.at(i) -> Fill(jet1.Pt(), eventWeight);
              if(printEventNumber) std::cout << eventNumber << ": " << jet0.Pt() << " > " << leadPt << ", " << jet1.Pt() << " > " << subleadPt << ", fabs(" << thisyStar << ") > " << yStarAnti << std::endl;
            }
            i++;
          }
        }
      }
    }

    // for(int i = 0; i < m_regions_mjj.size(); i++) {
      // m_regions_mjj.at(i) -> Fill(mjj, eventWeight);
      // add others
    // }

  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* /*eventInfo*/ ) {

  if(m_debug) std::cout << "IParticleHists: in execute " <<std::endl;

  //basic
  m_Pt_l ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt ->        Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_m ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_s ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Eta->        Fill( particle->eta(),       eventWeight );
  m_Phi->        Fill( particle->phi(),       eventWeight );
  m_M->          Fill( particle->m()/1e3,     eventWeight );
  m_E->          Fill( particle->e()/1e3,     eventWeight );
  m_Rapidity->   Fill( particle->rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {
    
  
    m_Px->  Fill( particle->p4().Px()/1e3,  eventWeight );
    m_Py->  Fill( particle->p4().Py()/1e3,  eventWeight );
    m_Pz->  Fill( particle->p4().Pz()/1e3,  eventWeight );


    m_Et ->        Fill( particle->p4().Et()/1e3,    eventWeight );
    m_Et_m ->      Fill( particle->p4().Et()/1e3,    eventWeight );
    m_Et_s ->      Fill( particle->p4().Et()/1e3,    eventWeight );
  } // fillKinematic

  return StatusCode::SUCCESS;
}




StatusCode IParticleHists::execute( const xAH::Particle* particle, float eventWeight, const xAH::EventInfo* /*eventInfo*/ ) {

  if(m_debug) std::cout << "IParticleHists: in execute " <<std::endl;

  const TLorentzVector& partP4 = particle->p4;

  //basic
  m_Pt_l ->      Fill( partP4.Pt(),    eventWeight );
  m_Pt ->        Fill( partP4.Pt(),    eventWeight );
  m_Pt_m ->      Fill( partP4.Pt(),    eventWeight );
  m_Pt_s ->      Fill( partP4.Pt(),    eventWeight );
  m_Eta->        Fill( partP4.Eta(),       eventWeight );
  m_Phi->        Fill( partP4.Phi(),       eventWeight );
  m_M->          Fill( partP4.M(),     eventWeight );
  m_E->          Fill( partP4.E(),     eventWeight );
  m_Rapidity->   Fill( partP4.Rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {
    m_Px->  Fill( partP4.Px(),  eventWeight );
    m_Py->  Fill( partP4.Py(),  eventWeight );
    m_Pz->  Fill( partP4.Pz(),  eventWeight );

    m_Et ->        Fill( partP4.Et(),    eventWeight );
    m_Et_m ->      Fill( partP4.Et(),    eventWeight );
    m_Et_s ->      Fill( partP4.Et(),    eventWeight );
  } // fillKinematic

  return StatusCode::SUCCESS;
}

