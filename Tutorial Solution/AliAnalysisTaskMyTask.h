/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"

class AliPIDResponse;
class AliMultSelection;

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliAODEvent*            fAOD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   fHistPt;        //! dummy histogram
        TH1F*                   fHistPVZ; 
        TH2F*                   fHistEtaPhi;
        TH2F*                   fHistPID;
        TH1F*                   fHistCentrality;
        TH2F*                   fHistPionPID;
        TH1F*                   fHistPionPt;
        TH1F*                   fHistPionEta;
        TH1F*                   fHistPionPhi;
        TH1F*                   fHistPionPtC;
        TH1F*                   fHistPionEtaC;
        TH1F*                   fHistPionPhiC;
        TH1F*                   fHistPionPtP;
        TH1F*                   fHistPionEtaP;
        TH1F*                   fHistPionPhiP;
  
	AliPIDResponse*         fPIDResponse;   //! pid response

        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};
#endif
