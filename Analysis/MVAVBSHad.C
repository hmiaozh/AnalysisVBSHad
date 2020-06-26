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


void MVAVBSHad(
  TString inputFileName, 
  TString extraString="" 
) {
  TMVA::Tools::Instance();

  gROOT->ProcessLine("TMVA::gConfig().GetVariablePlotting().fMaxNumOfAllowedVariablesForScatterPlots = 50");
  TFile *output_file;
  TMVA::Factory *factory;
  
  // Determine the input trees
  TFile *inputFile = TFile::Open(inputFileName,"READ");
  TTree *mvaTree = (TTree*)inputFile->Get("tree_vbs");
  
  // Initialize the factory
  TString trainName="BDT_VBSHad";
  output_file=TFile::Open(trainName+".root", "RECREATE");
  factory = new TMVA::Factory("TMVAMulticlass", output_file, "!V:!Silent:Color:DrawProgressBar:AnalysisType=Multiclass");
  //factory = new TMVA::Factory("bdt", output_file, "!V:!Silent:DrawProgressBar:AnalysisType=Classification");
  //factory = new TMVA::Factory("bdt", output_file, "!V:!Silent:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Multiclass");
 
  TMVA::DataLoader *dataloader=new TMVA::DataLoader("MitHadAnalysis");
  
  dataloader->AddTree(mvaTree, "ZZ", 1.0, "ana_category == 3 && mc == 3 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "ewkWZ", 1.0, "ana_category == 3 && mc == 4 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "qcdWZ", 1.0, "ana_category == 3 && mc == 5 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "others", 1.0, "ana_category == 3 && mc > 50 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "ZZ", 1.0, "ana_category == 3 && mc == 3 && evt % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "ewkWZ", 1.0, "ana_category == 3 && mc == 4 && evt % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "qcdWZ", 1.0, "ana_category == 3 && mc == 5 && evt % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "others", 1.0, "ana_category == 3 && mc > 50 && evt % 3 == 0", "test");

  dataloader->SetWeightExpression("abs(weight)", "ZZ");
  dataloader->SetWeightExpression("abs(weight)", "ewkWZ");
  dataloader->SetWeightExpression("abs(weight)", "qcdWZ");
  dataloader->SetWeightExpression("abs(weight)", "others");
  


  /*
  dataloader->AddTree(mvaTree, "Signal", 1.0, "ana_category == 3 && mc == 5 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "Background", 1.0, "ana_category == 3 && mc > 5 && evt % 3 != 0", "train");
  dataloader->AddTree(mvaTree, "Signal", 1.0, "ana_category == 3 && mc == 5 && evt % 3 == 0", "test");
  dataloader->AddTree(mvaTree, "Background", 1.0, "ana_category == 3 && mc > 5 && evt % 3 == 0", "test");

  dataloader->SetWeightExpression("abs(weight)", "Signal");
  dataloader->SetWeightExpression("abs(weight)", "Background");
  */


  dataloader->AddVariable("varMjj"    ,"varMjj"    , "", 'F');
  dataloader->AddVariable("varDetajj"    ,"varDetajj"    , "", 'F');
  dataloader->AddVariable("varDphijj"    ,"varDphijj"    , "", 'F');
  dataloader->AddVariable("varJet2Eta"    ,"varJet2Eta"    , "", 'F');
  dataloader->AddVariable("varJet2Pt"    ,"varJet2Pt"    , "", 'F');
  dataloader->AddVariable("j1Unc"    ,"j1Unc"    , "", 'F');
  dataloader->AddVariable("j2Unc"    ,"j2Unc"    , "", 'F');
  dataloader->AddVariable("varMVV"    ,"varMVV"    , "", 'F');
  dataloader->AddVariable("varPTVV"    ,"varPTVV"    , "", 'F');
  dataloader->AddVariable("varPTV1"    ,"varPTV1"    , "", 'F');
  dataloader->AddVariable("varPTV2"    ,"varPTV2"    , "", 'F');
  //dataloader->AddVariable("varDetaVV"    ,"varDetaVV"    , "", 'F');
  //dataloader->AddVariable("varPetaVV"    ,"varPetaVV"    , "", 'F');
  //dataloader->AddVariable("varEtaMinV"    ,"varEtaMinV"    , "", 'F');
  //dataloader->AddVariable("varEtaMaxV"    ,"varEtaMaxV"    , "", 'F');
  //dataloader->AddVariable("varCen"    ,"varCen"    , "", 'F');
  dataloader->AddVariable("varzepVB"    ,"varzepVB"    , "", 'F');
  //dataloader->AddVariable("varzepVV"    ,"varzepVV"    , "", 'F');
  dataloader->AddVariable("varDRVj"    ,"varDRVj"    , "", 'F');
  dataloader->AddVariable("varnormPTVVjj"    ,"varnormPTVVjj"    , "", 'F');
  //dataloader->AddVariable("varFW2j"    ,"varnormFW2j"    , "", 'F');
 

  /* 
  TString prepareOptions="NormMode=None";
    prepareOptions+=":SplitMode=Block"; // use e.g. all events selected by trainTreeEventSplitStr for training
    prepareOptions+=":MixMode=Random";
  dataloader->PrepareTrainingAndTestTree("", prepareOptions);
  */    


  dataloader->PrepareTrainingAndTestTree( "", "SplitMode=Block:MixMode=Random:NormMode=NumEvents:!V" );

  
  //TString hyperparameters=
  //"!H:!V:BoostType=AdaBoost:MinNodeSize=5%:NegWeightTreatment=IgnoreNegWeightsInTraining:SeparationType=MisClassificationError:NTrees=500:MaxDepth=3:AdaBoostBeta=0.12:nCuts=10000";
  //TString hyperparameters="!H:!V:NTrees=500:MinNodeSize=5%:MaxDepth=3:BoostType=Grad:Shrinkage=0.1:nCuts=30:PruneMethod=CostComplexity";
  //TString hyperparameters="!H:!V:NTrees=500:NegWeightTreatment=Pray:MinNodeSize=5%:MaxDepth=2:BoostType=Grad:Shrinkage=0.1:nCuts=30";

  TString hyperparameters="!H:!V:NTrees=500:NegWeightTreatment=Pray:SeparationType=MisClassificationError:MinNodeSize=5%:MaxDepth=2:BoostType=Grad:Shrinkage=0.01:nCuts=1000";
  //TString hyperparameters="!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2";

  factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", hyperparameters);

  hyperparameters="!H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+3:TestRate=5:!UseRegulator:EstimatorType=MSE";
  factory->BookMethod(dataloader, TMVA::Types::kMLP, "MLP", hyperparameters);

  //hyperparameters="H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1";
  //factory->BookMethod(dataloader, TMVA::Types::kFDA, "FDA_GA",  hyperparameters);


  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
  output_file->Close();
}
