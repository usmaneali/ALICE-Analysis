/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TMath.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"
#include "AliPIDResponse.h"
#include "AliMultSelection.h"
#include "AliCentrality.h"

class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    fAOD(0), fOutputList(0), fHistPt(0), fHistPVZ(0), fHistEtaPhi(0), fPIDResponse(0),
    fHistPID(0), fHistCentrality(0), fHistPionPID(0), fHistPionPt(0), fHistPionEta(0), fHistPionPhi(0), 
    fHistPionPtC(0), fHistPionEtaC(0), fHistPionPhiC(0), fHistPionPtP(0), fHistPionEtaP(0),
    fHistPionPhiP(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0), fHistPVZ(0), fHistEtaPhi(0), fPIDResponse(0),
    fHistPID(0), fHistCentrality(0), fHistPionPID(0), fHistPionPt(0), fHistPionEta(0), fHistPionPhi(0), 
    fHistPionPtC(0), fHistPionEtaC(0), fHistPionPhiC(0), fHistPionPtP(0), fHistPionEtaP(0),
    fHistPionPhiP(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //

    AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
    if (man) {
    AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
    if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
    }

    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)

    // example of a histogram
    fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 10);       // create your histogram
    fHistPVZ = new TH1F("fHistPVZ", "Primary Vertex along the Z-axis", 100, -25 ,25); // Histogram for Primary Vertex along the Z-axis
    fHistEtaPhi = new TH2F("fHistEtaPhi", "Eta vs Phi", 100, -1.5, 1.5, 100, 0, 2*TMath::Pi()); // Eta vs Phi
    fHistPID = new TH2F("fHistPID", "Particle identification", 100, 0, 10, 100, 0, 1000); //track momentum vs TPC signal for all particles
    fHistCentrality = new TH1F("fHistCentrality", "fHistCentrality", 100, 0, 100);

    // for pions
    fHistPionPID = new TH2F("fHistPionPID", "Pion signal", 100, 0, 10, 100, 0, 1000); //track momentum vs TPC signal for pions
    fHistPionPt = new TH1F("fHistPionPt", "Pt of pions", 100, 0, 10);          // Transverse Momentum for pions
    fHistPionEta = new TH1F("fHistPionEta", "Eta of pions", 100, -1.5, 1.5);       // Psedudorapidity for pions
    fHistPionPhi = new TH1F("fHistPionPhi", "Phi of pions", 100, 0, 2*TMath::Pi());  // Azimuthal angle for pions

    // for pions with centrality cut (0 < centrality < 10)
    fHistPionPtC = new TH1F("fHistPionPtC", "Pt of pions (0 < centrality < 10)", 100, 0, 10);          // Transverse Momentum for pions
    fHistPionEtaC = new TH1F("fHistPionEtaC", "Eta of pions (0 < centrality < 10)", 100, -1.5, 1.5);       // Psedudorapidity for pions
    fHistPionPhiC = new TH1F("fHistPionPhiC", "Phi of pions (0 < centrality < 10)", 100, 0, 2*TMath::Pi());  // Azimuthal angle for pions

    // for pions with centrality cut (50 < centrality < 60)
    fHistPionPtP = new TH1F("fHistPionPtP", "Pt of pions (50 < centrality < 60)", 100, 0, 10);          // Transverse Momentum for pions
    fHistPionEtaP = new TH1F("fHistPionEtaP", "Eta of pions (50 < centrality < 60)", 100, -1.5, 1.5);       // Psedudorapidity for pions
    fHistPionPhiP = new TH1F("fHistPionPhiP", "Phi of pions (50 < centrality < 60)", 100, 0, 2*TMath::Pi());  // Azimuthal angle for pions

    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
                                        // your histogram in the output file, add it to the list!

    fOutputList->Add(fHistPVZ);
    fOutputList->Add(fHistEtaPhi);
    fOutputList->Add(fHistPID);
    fOutputList->Add(fHistCentrality); 
    fOutputList->Add(fHistPionPID);
    fOutputList->Add(fHistPionPt);
    fOutputList->Add(fHistPionEta);
    fOutputList->Add(fHistPionPhi);
    fOutputList->Add(fHistPionPtC);
    fOutputList->Add(fHistPionEtaC);
    fOutputList->Add(fHistPionPhiC);
    fOutputList->Add(fHistPionPtP);
    fOutputList->Add(fHistPionEtaP);
    fOutputList->Add(fHistPionPhiP);
    
    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{
    // user exec
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
                                                        // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
        // example part: i'll show how to loop over the tracks in an event 
        // and extract some information from them which we'll store in a histogram

    float vertexZ = fAOD->GetPrimaryVertex()->GetZ();
    if(vertexZ <= 10 && vertexZ >= -10)
    {
        fHistPVZ->Fill(vertexZ);
    }

    Float_t centrality(0);
    AliMultSelection *multSelection =static_cast<AliMultSelection*>(fAOD->FindListObject("MultSelection"));
    if(multSelection) centrality = multSelection->GetMultiplicityPercentile("V0M");
    fHistCentrality->Fill(centrality);

    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event
    for(Int_t i(0); i < iTracks; i++)                  // loop ove rall these tracks
    {   
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));          // get a track (type AliAODTrack) from the event
        if(!track || !track->TestFilterBit(1)) continue;                            // if we failed, skip this track
        fHistPt->Fill(track->Pt());                     // plot the pt value of the track in a histogram
        fHistEtaPhi->Fill(track->Eta(), track->Phi());  // plot the pseudorapidity and azimuthal angle of a track in 2D histogram  
	    fHistPID->Fill(track->P(), track->GetTPCsignal());
        
        //fill histograms for pions
        if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track, AliPID::kPion)) < 3 ) 
        {
	        fHistPionPID->Fill(track->P(), track->GetTPCsignal());
	        fHistPionPt->Fill(track->Pt());
	        fHistPionEta->Fill(track->Eta());
	        fHistPionPhi->Fill(track->Phi());

            // centrality selection
	        if(centrality > 0 && centrality < 10) 
            {
	            fHistPionPtC->Fill(track->Pt());
	            fHistPionEtaC->Fill(track->Eta());
	            fHistPionPhiC->Fill(track->Phi());
	        } 
            else if (centrality > 50 && centrality < 60) 
            {
	            fHistPionPtP->Fill(track->Pt());
	            fHistPionEtaP->Fill(track->Eta());
	            fHistPionPhiP->Fill(track->Phi());   
	        }
        }
    }                                                   // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
