// \class JetToTracksAssociator JetToTracksAssociator.cc 
//
// Original Author:  Andrea Rizzi
//         Created:  Wed Apr 12 11:12:49 CEST 2006
// Accommodated for Jet Package by: Fedor Ratnikov Jul. 30, 2007
// $Id: JetToTracksAssociator.cc,v 1.1 2007/07/31 00:34:54 fedor Exp $
//
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/JetToTracksAssociation.h"

#include "JetToTracksAssociator.h"

JetToTracksAssociator::JetToTracksAssociator(const edm::ParameterSet& fConfig)
  : mJets (fConfig.getParameter<edm::InputTag> ("jets")),
    mTracks (fConfig.getParameter<edm::InputTag> ("tracks")),
    mAssociator (fConfig.getParameter<double> ("coneSize"))
{
  produces<reco::JetToTracksAssociation::Container> ();
}

JetToTracksAssociator::~JetToTracksAssociator() {}

void JetToTracksAssociator::produce(edm::Event& fEvent, const edm::EventSetup& fSetup) {
  edm::Handle <edm::View <reco::Jet> > jets_h;
  fEvent.getByLabel (mJets, jets_h);
  edm::Handle <reco::TrackCollection> tracks_h;
  fEvent.getByLabel (mTracks, tracks_h);
  
  std::auto_ptr<reco::JetToTracksAssociation::Container> jetTracks (new reco::JetToTracksAssociation::Container);

  // format inputs
  std::vector <edm::RefToBase<reco::Jet> > allJets;
  allJets.reserve (jets_h->size());
  for (unsigned i = 0; i < jets_h->size(); ++i) allJets.push_back (jets_h->refAt(i));
  std::vector <reco::TrackRef> allTracks;
  allTracks.reserve (tracks_h->size());
  for (unsigned i = 0; i < tracks_h->size(); ++i) allTracks.push_back (reco::TrackRef (tracks_h, i));
  // run algo
  mAssociator.produce (&*jetTracks, allJets, allTracks);
  // store output
  fEvent.put (jetTracks);
}
