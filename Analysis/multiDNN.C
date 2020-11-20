#include <TROOT.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TMVA/Types.h>
#include <TMVA/TMVAMultiClassGui.h>
#include <TMVA/Tools.h>

#include <TFile.h>
#include <TCut.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>


void multiDNN(
  TString inputFileName, 
  TString extraString="" 
) {
  TMVA::Tools::Instance();

  gROOT->ProcessLine("TMVA::gConfig().GetVariablePlotting().fMaxNumOfAllowedVariablesForScatterPlots = 50");
  TFile *output_file;
  TMVA::Factory *factory;
  
  // Determine the input trees
  TFile *inputFile = TFile::Open(inputFileName,"READ");
  TTree *mvaTree = (TTree*)inputFile->Get("tree_resTag");

  // Initialize the factory
  TString trainName="resTag_DNN_multi_RMET";
  output_file=TFile::Open(trainName+".root", "RECREATE");
  factory = new TMVA::Factory("resTag_DNN_multi_RMET", output_file, "!V:!Silent:Color:DrawProgressBar:AnalysisType=Multiclass");
 
  TMVA::DataLoader *dataloader=new TMVA::DataLoader("resTag_multi");
  

  dataloader->AddTree(mvaTree, "All OK", 1.0, "Allmatch == 1 && EvtNum % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "V OK F NOT", 1.0, "Vmatch == 1 && Allmatch == 0 && EvtNum % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "F OK V NOT", 1.0, "fmatch == 1 && Allmatch == 0 && EvtNum % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "All NOT", 1.0, "Vmatch == 0 && fmatch == 0 && EvtNum % 3 != 0", "train");

  dataloader->AddTree(mvaTree, "All OK", 1.0, "Allmatch == 1 && EvtNum  % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "V OK F NOT", 1.0, "Vmatch == 1 && Allmatch == 0 && EvtNum % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "F OK V NOT", 1.0, "fmatch == 1 && Allmatch == 0 && EvtNum % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "All NOT", 1.0, "Vmatch == 0 && fmatch == 0 && EvtNum % 3 == 0", "test");

  dataloader->SetWeightExpression("abs(Weights)", "All OK");
  dataloader->SetWeightExpression("abs(Weights)", "V OK F NOT");
  dataloader->SetWeightExpression("abs(Weights)", "F OK V NOT");
  dataloader->SetWeightExpression("abs(Weights)", "All NOT");



  dataloader->AddVariable("j1_pT", "jet1 pT"    , "", 'F');
  dataloader->AddVariable("j1_Eta", "jet1 eta"    , "", 'F');
  dataloader->AddVariable("j1_Unc", "jet1 Unc"    , "", 'F');
  dataloader->AddVariable("j1_axis2", "jet1 axis2"    , "", 'F');
  dataloader->AddVariable("j1_ptD", "jet1 ptD"    , "", 'F');

  dataloader->AddVariable("j2_pT", "jet2 pT"    , "", 'F');
  dataloader->AddVariable("j2_Unc", "jet2 Unc"    , "", 'F');
  dataloader->AddVariable("j2_axis2", "jet2 axis2"    , "", 'F');
  dataloader->AddVariable("j2_ptD", "jet2 ptD"    , "", 'F');


  dataloader->AddVariable("j3_pT", "jet3 pT"    , "", 'F');
  dataloader->AddVariable("j3_Eta", "jet3 eta"    , "", 'F');
  dataloader->AddVariable("j3_Unc", "jet3 Unc"    , "", 'F');
  dataloader->AddVariable("j3_axis2", "jet3 axis2"    , "", 'F');
  dataloader->AddVariable("j3_ptD", "jet3 ptD"    , "", 'F');
  dataloader->AddVariable("j4_pT", "jet4 pT"    , "", 'F');
  dataloader->AddVariable("j4_Unc", "jet4 Unc"    , "", 'F');
  dataloader->AddVariable("j4_axis2", "jet4 axis2"    , "", 'F');
  dataloader->AddVariable("j4_ptD", "jet4 ptD"    , "", 'F');

  dataloader->AddVariable("DEta_f12", "DEta_f12"    , "", 'F');
  dataloader->AddVariable("DEta_v34", "DEta_v34"    , "", 'F');
  dataloader->AddVariable("DPhi_f12", "DPhi_f12"    , "", 'F');
  dataloader->AddVariable("DPhi_v34", "DPhi_v34"    , "", 'F');

  dataloader->AddVariable("DR_v34", "DR_v34"    , "", 'F');
  dataloader->AddVariable("M_v34", "M_v34"    , "", 'F');


  TCut preselectionCut = "";
//  dataloader->PrepareTrainingAndTestTree(preselectionCut, "SplitMode=Block:MixMode=Random:NormMode=NumEvents:!V" );
  dataloader->PrepareTrainingAndTestTree(preselectionCut, "SplitMode=Block:MixMode=Random:NormMode=EqualNumEvents:!V" );


        TString layoutString ("Layout=TANH|128,RELU|128,SIGMOID|128,LINEAR");


         // Training strategies.
         TString training0("LearningRate=1e-1,Momentum=0.9,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
         TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
         TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
         TString training3("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L1,"
                           "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");

         TString trainingStrategyString ("TrainingStrategy=");
         trainingStrategyString += training0 + "|" + training1 + "|" + training2;

         // General Options.
         TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                             "WeightInitialization=XAVIERUNIFORM");
         dnnOptions.Append (":"); dnnOptions.Append (layoutString);
         dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

         // Standard implementation, no dependencies.
         TString stdOptions = dnnOptions + ":Architecture=CPU";
         factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN", stdOptions);


  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
  output_file->Close();
}

