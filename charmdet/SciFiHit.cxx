#include "SciFiHit.h"
#include <iostream>
#include "TGeoBBox.h"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoShape.h"
#include "TVector3.h"

//SciFiHit::SciFiHit(Int_t detID, Float_t digi) : ShipHit(detID, digi) {}
//SciFiHit::SciFiHit(Int_t detID, Float_t digi, uint32_t hitTime, uint32_t fineTime, uint16_t ch, uint16_t boardId, uint16_t module, int16_t flag, bool trigFlag): ShipHit(detID, digi), hitTime(hitTime), fineTime(fineTime), ch(ch), boardId(boardId), module(module), flag(flag), trigFlag(trigFlag){}
SciFiHit::SciFiHit(Int_t detID, Float_t digi, uint32_t hitTime, uint32_t fineTime, uint16_t amp, uint16_t ch, uint16_t sticId, uint16_t boardId, bool flag): ShipHit(detID, digi), hitTime(hitTime), fineTime(fineTime), amp(amp), ch(ch), sticId(sticId), boardId(boardId), flag(flag){}


void SciFiHit::EndPoints(TVector3 &vbot, TVector3 &vtop) {
// method to get strip endpoints from TGeoNavigator
  Int_t statnb = fDetectorID/10000;
  Int_t orientationnb = (fDetectorID-statnb*10000)/1000;  //1=vertical, 0=horizontal
  if (orientationnb > 1) {
     std::cout << "SciFi::StripEndPoints, not a sensitive volume "<<fDetectorID<<std::endl;
     return;
  }
  TString stat="VMuonBox_1/VSensitive";stat+=+statnb;stat+="_";stat+=statnb;
  TString striptype;
  if (orientationnb == 0) {
    striptype = "Hstrip_";
    if (fDetectorID%1000==116 || fDetectorID%1000==1){striptype = "Hstrip_ext_";}
  }
  if (orientationnb == 1) {
    striptype = "Vstrip_";
    if (fDetectorID%1000 == 184){striptype = "Vstrip_L_";}
    if (fDetectorID%1000 == 1){striptype = "Vstrip_R_";}
  }  TGeoNavigator* nav = gGeoManager->GetCurrentNavigator();
  TString path = "";path+="/";path+=stat;path+="/"+striptype;path+=fDetectorID;
  Bool_t rc = nav->cd(path);
  if (not rc){
       std::cout << "SciFi::StripEndPoints, TGeoNavigator failed "<<path<<std::endl;
       return;
  }
  TGeoNode* W = nav->GetCurrentNode();
  TGeoBBox* S = dynamic_cast<TGeoBBox*>(W->GetVolume()->GetShape());
  Double_t top[3] = {0,0,S->GetDZ()};
  Double_t bot[3] = {0,0,-S->GetDZ()};
  Double_t Gtop[3],Gbot[3];
  nav->LocalToMaster(top, Gtop);
  nav->LocalToMaster(bot, Gbot);
  vtop.SetXYZ(Gbot[0],Gbot[1],Gbot[2]);
  vbot.SetXYZ(Gtop[0],Gtop[1],Gtop[2]);
}

ClassImp(SciFiHit)
