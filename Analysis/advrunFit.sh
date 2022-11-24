#!/bin/sh
cat=$1
var=$2
year=$3
quote=$4
region=$5

#### datacards
echo "produce cards"
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "MasscorrFlower_BBtag_$year.root" -y $year -q $quote
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "ResultV3_QCDSF_DNN_MET_"$year"_"$region".root" -y $year -q $quote
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "BHAD_sep3_SR_$year.root" -y $year -q $quote
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "HAD"$region"_"$year"_sep12.root" -y $year -q $quote -r $region
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "MET_all_jul31_$year.root" -y $year -q $quote
#python script/bwsVBSHad.py -c "$cat" -s "$var" -i "OptimizeV0_V2_MET_$year.root" -y $year -q $quote

echo "start t2w"
#combineCards.py -S 


#text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
#--PO 'map=.*/ZZEWK:r_s0[1,0,5]' \
#--PO 'map=.*/osWWEWK:r_s1[1,0,5]' \
#--PO 'map=.*/ssWWEWK:r_s2[1,0,5]' \
#Datacards/MAR24/cms_vbshad_$year\_$quote\_$var\_$cat\.txt -o Datacards/MAR24/workspace_$year\_$var\_$cat\.root


text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose --PO 'map=.*/VV_AQGC_EWK:r_s0[1,0,5]' Datacards/MAR24/cms_vbshad_$year\_$quote\_aqgc_ft2_$var\_$cat\_$region\.txt -o Datacards/MAR24/workspace_$year\_$var\_$cat\_$region\_VVEWK.root --keyword-value VALUE=0p00

echo "done t2w"

echo "significance"

combine Datacards/MAR24/workspace_$year\_$var\_$cat\_$region\_VVEWK.root -M Significance -n $cat_significance_$var\_$region\_VVEWK --setParameters r_s0=1 -t -1

echo "FitDiagnostics"

combine -M FitDiagnostics -t -1 Datacards/MAR24/workspace_$year\_$var\_$cat\_$region\_VVEWK.root --robustFit=1  --X-rtd MINIMIZER_analytic --saveShapes --saveWithUncertainties --saveNormalizations  --setParameters r_s0=1 --rMin -10 --rMax 10 -n MAR24_$year\_$var\_$cat\_$region\_VVEWK

echo "test"
#combine Datacards/MAR24/workspace_$year\_$var\_$cat.root -M Significance -n $cat_significance_$var\_ZZEWK --setParameters r_s0=1 -t -1
#combine Datacards/MAR24/workspace_$year\_$var\_$cat.root -M Significance -n $cat_significance_$var\_osWWEWK --setParameters r_s1=1 -t -1
#combine Datacards/MAR24/workspace_$year\_$var\_$cat.root -M Significance -n $cat_significance_$var\_ssWWEWK --setParameters r_s2=1 -t -1


#combine -M AsymptoticLimits workspace_$2.root --newExpected=1 -n $2${ADDTEXT} --rMax 1.0 --noFitAsimov
#combine -M AsymptoticLimits Datacards/Nano/workspace_$var_$cat_ZZEWK.root --X-rtd MINIMIZER_analytic  --cminDefaultMinimizerType=Minuit2  --rMax=10.000000  --rMin=-10.000000
#combine Datacards/Nano/workspace_$var_$cat.root -M Significance -n $cat_significance_$var_ZZEWK --setParameters r_s0=1,r_s1=1,r_s2=1 -t -1 --redefineSignalPOIs r_s0
#combine Datacards/Nano/workspace_$var_$cat.root -M Significance -n $cat_significance_$var_WZEWK --setParameters r_s0=1,r_s1=1,r_s2=1 -t -1 --redefineSignalPOIs r_s1
#combine Datacards/Nano/workspace_$var_$cat.root -M Significance -n $cat_significance_$var_WZQCD --setParameters r_s0=1,r_s1=1,r_s2=1 -t -1 --redefineSignalPOIs r_s2

#combine -M MultiDimFit Datacards/Nano/workspace_$var_$cat.root -n $cat_2D_$var_WZ -P r_s1 -P r_s2 --floatOtherPOIs 1 --algo grid --points=10000 -t -1
