//! Detector status plots
#include "DetCheckHistos.hpp"

DetCheckHistos::DetCheckHistos(TFile* localfile, string treename) {
    cout << "Running in single file mode. You gave me the file so the filename and path doesnt matter" << endl;
    file = localfile;
    tree = (TTree*)file->Get(treename.c_str());
    cout << "Number of entries in TTree= " << tree->GetEntriesFast() << endl;
    LanlPlotter.SetTree(tree);
    VecBuilder(&LanlPlotter);
    cout << "Simplified Tree Vectors Built" << endl;
}

DetCheckHistos::DetCheckHistos(int startfile, int stopfile, string treeName) {
    string last4ofTreeName = treeName.substr(treeName.length()-4);
    if (strcmp(last4ofTreeName.c_str(),"ornl") == 0 ){
        SetLanlTreeBool(false);
    } else {
        SetLanlTreeBool(true);
    }
    chain = new TChain(treeName.c_str());
    cout << "Running in TChain Mode using \"" + treeName + "\" as the tree name" << endl;
    cout << "     Filename needs to match \"./Stage0_Histograms_Run_*.root\" to be added to the tree" << endl;
    for (int it = startfile; it <= stopfile; ++it) {
        string file2Add = "Stage0_Histograms_Run_" + to_string(it) + ".root";
        chain->Add(file2Add.c_str());
    }
    LanlPlotter.SetTree(chain);
    VecBuilder(&LanlPlotter, GetLanlTreeBool());
    cout << "Simplified Tree Vectors Built" << endl;
}

int DetCheckHistos::TreeWalker() {
    if (DataProcessed) {
        cout << "You already did this, ya dummy" << endl;
        return 1;
    }

    Make_Single_Type_Histos(odesaChans, "odesa", odesaPlots);
    Make_Single_Type_Histos(stilbeneChans, "stilbene", stilbenePlots);
    Make_Single_Type_Histos(hpgeChans, "hpge", hpgePlots);
    Make_Single_Type_Histos(siChans, "si", siPlots);

    Make_Mike_Histos(odesaChans, "odesa", odesaPlots);
    Make_Mike_Histos(hpgeChans, "hpge", hpgePlots);

    BuildNeutronGatesVector("odesa", odesa_gates);

    // comboPlots = new TObjArray;
    comboPlots.Add(new TH2D("Si16_hpge29", "Si16 vs hpge29", 600, 0, 6000, 6000, 0, 6000));
    comboPlots.Add(new TH2D("Si16_hpge31", "Si16 vs hpge31", 600, 0, 6000, 6000, 0, 6000));
    comboPlots.Add(new TH2D("SiSum_hpge29", "Si Forward Stack Sum vs hpge29", 600, 0, 6000, 6000, 0, 6000));

    if (LanlPlotter.IsChain()) {
        comboPlots.Add(new TH1D("hID_list", "List of hID bin 16 content for the files, inorder. Skip bin 0 ", chain->GetNtrees() + 2, 1, chain->GetNtrees() + 2));
    } else {
        comboPlots.Add(new TH1D("hID_ref", "Reference of the hID bin 16 content, skipping bin 0", 2, 0, 2));
    };

    std::cout << "Plots Made. Starting Compute" << endl;
    long eventCounter = 0;
    int currentTreeNumber = -1;

    // //! Filling hID for non chain cases, only need it once since we wont move outside of this file.
    if (!LanlPlotter.IsChain()){
        double hID_Value;
        if (file->FindObject("hID")) {
            hID_Value = ((TH1*)(file->FindObject("hID")))->GetBinContent(16);
            ((TH1*)comboPlots.FindObject("hID_ref"))->Fill(1, hID_Value);
        }
    }
    long totalEntries = LanlPlotter.GetEntries();
    while (LanlPlotter.Next()) {
        if (LanlPlotter.IsChain() && chain->GetTreeNumber() != currentTreeNumber) {
            cout << "New TTree, pulling hID value" << endl;
            double hID_Value = (((TH1*)(chain->GetFile())->FindObject("hID")))->GetBinContent(16);
            int curFileNumber = (chain->GetFileNumber()) + 1;  //! Shift away from 0
            ((TH1*)comboPlots.FindObject("hID_list"))->Fill(curFileNumber, hID_Value);
            currentTreeNumber = chain->GetTreeNumber();
        }

        for (auto iter = odesa_has_a_neutron.begin(); iter != odesa_has_a_neutron.end(); ++iter) {
            (*iter) = false;
        }

        for (unsigned itOdesa = 0; itOdesa < odesa.size(); ++itOdesa) {
            double efast = (**odesa.at(itOdesa).efast);
            double eslow = (**odesa.at(itOdesa).eslow);
            double twrap = (**odesa.at(itOdesa).twrap);
            double pheight = (**odesa.at(itOdesa).pheight);
            double psd = efast / eslow;

            detInfo& curGates = odesa_gates.at(itOdesa);

            string key2find = "ode" + to_string(odesa.at(itOdesa).locNum) + "_psd_int";
            TH2* plot2d = (TH2*)odesaPlots.FindObject(key2find.c_str());
            if (plot2d && pheight < 15000) {
                plot2d->Fill(eslow, psd);
            }
            key2find = "ode" + to_string(odesa.at(itOdesa).locNum) + "_psd_pha";
            plot2d = (TH2*)odesaPlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(pheight, psd);
            }
            key2find = "ode" + to_string(odesa.at(itOdesa).locNum) + "_tof_eslow";
            plot2d = (TH2*)odesaPlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(twrap, eslow);
            }

            if (eslow * curGates.eslow_cal_m > curGates.psd_eslow_thresh && efast / eslow > curGates.psd_low && efast / eslow < curGates.psd_high && pheight < curGates.pheight_clip) {
                odesa_has_a_neutron.at(itOdesa) = true;
                key2find = "ode" + to_string(odesa.at(itOdesa).locNum) + "_tofvE_mike";
                plot2d = (TH2*)odesaPlots.FindObject(key2find.c_str());
                if (plot2d) {
                    plot2d->Fill(twrap, eslow);
                }
            }
        }
        for (unsigned itStilbene = 0; itStilbene < stilbene.size(); ++itStilbene) {
            double efast = (**stilbene.at(itStilbene).efast);
            double eslow = (**stilbene.at(itStilbene).eslow);
            double twrap = (**stilbene.at(itStilbene).twrap);
            double pheight = (**stilbene.at(itStilbene).pheight);
            double psd = efast / eslow;
            string key2find = "sti" + to_string(stilbene.at(itStilbene).locNum) + "_psd_int";
            TH2* plot2d = (TH2*)stilbenePlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(eslow, psd);
            }
            key2find = "sti" + to_string(stilbene.at(itStilbene).locNum) + "_psd_pha";
            plot2d = (TH2*)stilbenePlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(pheight, psd);
            }
            key2find = "sti" + to_string(stilbene.at(itStilbene).locNum) + "_tof_eslow";
            plot2d = (TH2*)stilbenePlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(twrap, eslow);
            }
        }

        for (unsigned itHpge = 0; itHpge < hpge.size(); ++itHpge) {
            double eslow = (**hpge.at(itHpge).eslow);
            double twrap = (**hpge.at(itHpge).twrap);
            string key2find = "hpge" + to_string(hpge.at(itHpge).locNum) + "_e";
            TH1* plot1d = (TH1*)hpgePlots.FindObject(key2find.c_str());
            if (plot1d) {
                plot1d->Fill(eslow);
            }
            key2find = "hpge" + to_string(hpge.at(itHpge).locNum) + "_e_tof";
            TH2* plot2d = (TH2*)hpgePlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(twrap, eslow);
            }
            for (unsigned itNe = 0; itNe < odesa_has_a_neutron.size(); ++itNe) {
                key2find = "hpge" + to_string(hpge.at(itHpge).locNum) + "_e_ode" + to_string(odesaChans.at(itNe)) + "_ncut_mike";
                plot1d = (TH1*)hpgePlots.FindObject(key2find.c_str());
                if (plot1d) {
                    plot1d->Fill(eslow);
                }
            }
        }
        pair<double, double> stack1_deTKE = {0.0, 0.0}, stack2_deTKE = {0.0, 0.0};
        for (unsigned itSi = 0; itSi < si.size(); ++itSi) {
            double eslow = (**si.at(itSi).eslow);
            double twrap = (**si.at(itSi).twrap);

            if (floor(si.at(itSi).detNum / 3) == 0) {
                if (si.at(itSi).locNum == siChans.at(0)) {
                    stack1_deTKE.first = eslow;
                }
                stack1_deTKE.second += eslow;
            } else if (floor(si.at(itSi).detNum / 3) == 1) {
                if (si.at(itSi).locNum == siChans.at(3)) {
                    stack2_deTKE.first = eslow;
                }
                stack2_deTKE.second += eslow;
            }

            string key2find = "si" + to_string(si.at(itSi).locNum) + "_e";
            TH1* plot1 = (TH1*)hpgePlots.FindObject(key2find.c_str());
            if (plot1) {
                plot1->Fill(eslow);
            }
            key2find = "si" + to_string(si.at(itSi).locNum) + "_e_tof";
            TH2* plot2d = (TH2*)hpgePlots.FindObject(key2find.c_str());
            if (plot2d) {
                plot2d->Fill(twrap, eslow);
            }
        }
        ((TH2*)siPlots.FindObject("si_stack1_deTKE"))->Fill(stack1_deTKE.first, stack1_deTKE.second);
        ((TH2*)siPlots.FindObject("si_stack2_deTKE"))->Fill(stack2_deTKE.first, stack2_deTKE.second);

        if (eventCounter % 10000 == 0) {
            cout << "\rEntries counter: " << eventCounter << std::setprecision(3)<<" (" << (eventCounter/(double)totalEntries)*100 <<" %)" << std::flush;
        }
        eventCounter++;
    }

    cout << endl
         << "Done:: Scanned " << eventCounter << " out of " << (long)totalEntries << " entries" << endl;
    DataProcessed = true;
    return 0;
}

void DetCheckHistos::Plot2DirectoryDump(TObjArray* array) {
    for (auto it = array->begin(); it != array->end(); ++it) {
        gDirectory->Add((*it));
    }
}

int DetCheckHistos::Make_Single_Type_Histos(vector<int> Chans, string dettype, TObjArray& objarray) {
    if (strcmp(dettype.c_str(), "odesa") == 0 || strcmp(dettype.c_str(), "stilbene") == 0) {
        string detPrefix = dettype.substr(0, 3);
        for (unsigned chanIT = 0; chanIT < Chans.size(); ++chanIT) {
            string PSD1_key = detPrefix + to_string(Chans.at(chanIT)) + "_psd_int";
            string PSD1_title = detPrefix + to_string(Chans.at(chanIT)) + " PSD vs eSlow";
            string PSD2_key = detPrefix + to_string(Chans.at(chanIT)) + "_psd_pha";
            string PSD2_title = detPrefix + to_string(Chans.at(chanIT)) + " PSD vs pHeight";
            string tof_eslow_key = detPrefix + to_string(Chans.at(chanIT)) + "_tof_eslow";
            string tof_eslow_title = detPrefix + to_string(Chans.at(chanIT)) + " ToF vs eSlow";

            objarray.Add(new TH2D(PSD1_key.c_str(), PSD1_title.c_str(), 1000, 0, 5000, 1000, 0, 1));
            objarray.Add(new TH2D(PSD2_key.c_str(), PSD2_title.c_str(), 1000, 0, 5000, 1000, 0, 1));
            objarray.Add(new TH2D(tof_eslow_key.c_str(), tof_eslow_title.c_str(), 1000, 0, 200, 3000, 0, 3000));
        }
    } else if (strcmp(dettype.c_str(), "hpge") == 0) {
        for (unsigned chanIT = 0; chanIT < Chans.size(); ++chanIT) {
            string hpge_key = "hpge" + to_string(Chans.at(chanIT)) + "_e";
            string hpge_title = "hpge " + to_string(Chans.at(chanIT)) + " eSlow";

            // string hpge_key_ncut = "hpge" + to_string(Chans.at(chanIT)) + "_e_ncut";
            // string hpge_title_ncut = "hpge " + to_string(Chans.at(chanIT)) + " eSlow_ncut";

            string hpgeVtof_key = "hpge" + to_string(Chans.at(chanIT)) + "_e_tof";
            string hpgeVtof_title = "hpge " + to_string(Chans.at(chanIT)) + " eSlow vs ToF";

            // objarray.Add(new TH1D(hpge_key_ncut.c_str(),hpge_title_ncut.c_str(),6000,0,6000));
            objarray.Add(new TH1D(hpge_key.c_str(), hpge_title.c_str(), 6000, 0, 6000));
            objarray.Add(new TH2D(hpgeVtof_key.c_str(), hpgeVtof_title.c_str(), 6000, 0, 6000, 1000, 0, 200));
        }
    } else if (strcmp(dettype.c_str(), "si") == 0) {
        for (unsigned chanIT = 0; chanIT < Chans.size(); ++chanIT) {
            string si_key = "si_" + to_string(Chans.at(chanIT)) + "_e";
            string si_title = "si_" + to_string(Chans.at(chanIT)) + " eSlow";
            string siVtof_key = "si_" + to_string(Chans.at(chanIT)) + "_e_tof";
            string siVtof_title = "si_" + to_string(Chans.at(chanIT)) + " eSlow vs ToF";
            objarray.Add(new TH1D(si_key.c_str(), si_title.c_str(), 600, 0, 6000));
            objarray.Add(new TH2D(siVtof_key.c_str(), siVtof_title.c_str(), 600, 0, 6000, 1000, 0, 200));
        }

        string sideE_key = "si_stack1_deTKE";
        string sideE_title = "si stack1  dE vs TKE";
        objarray.Add(new TH2D(sideE_key.c_str(), sideE_title.c_str(), 600, 0, 6000, 1200, 0, 12000));
        sideE_key = "si_stack2_deTKE";
        sideE_title = "si stack2  dE vs TKE";
        objarray.Add(new TH2D(sideE_key.c_str(), sideE_title.c_str(), 600, 0, 6000, 1200, 0, 12000));

    } else {
        return 1;
    }

    return 0;
}

void DetCheckHistos::VecBuilder(TTreeReader* Reader, bool lanltree) {
    stilbene = std::vector<detdata>(8, detdata());
    odesa = std::vector<detdata>(10, detdata());

    if (lanltree) {
        hpge = std::vector<detdata>(2, detdata());
        si = std::vector<detdata>(6, detdata());
    }

    for (unsigned it = 0; it < odesaChans.size(); ++it) {
        odesa.at(it).detNum = it;
        odesa.at(it).locNum = odesaChans.at(it);
        string eslow_name = "eslow_" + to_string(odesaChans.at(it));
        odesa.at(it).eslow = new TTreeReaderValue<Double_t>{(*Reader), eslow_name.c_str()};
        string efast_name = "efast_" + to_string(odesaChans.at(it));
        odesa.at(it).efast = new TTreeReaderValue<Double_t>{(*Reader), efast_name.c_str()};
        string twrap_name = "twrap_" + to_string(odesaChans.at(it));
        odesa.at(it).twrap = new TTreeReaderValue<Double_t>{(*Reader), twrap_name.c_str()};
        string pheight_name = "pheight_" + to_string(odesaChans.at(it));
        odesa.at(it).pheight = new TTreeReaderValue<Double_t>{(*Reader), pheight_name.c_str()};
    }

    for (unsigned it = 0; it < stilbeneChans.size(); ++it) {
        stilbene.at(it).detNum = it;
        stilbene.at(it).locNum = stilbeneChans.at(it);
        string eslow_name = "eslow_" + to_string(stilbeneChans.at(it));
        stilbene.at(it).eslow = new TTreeReaderValue<Double_t>{(*Reader), eslow_name.c_str()};
        string efast_name = "efast_" + to_string(stilbeneChans.at(it));
        stilbene.at(it).efast = new TTreeReaderValue<Double_t>{(*Reader), efast_name.c_str()};
        string twrap_name = "twrap_" + to_string(stilbeneChans.at(it));
        stilbene.at(it).twrap = new TTreeReaderValue<Double_t>{(*Reader), twrap_name.c_str()};
        string pheight_name = "pheight_" + to_string(stilbeneChans.at(it));
        stilbene.at(it).pheight = new TTreeReaderValue<Double_t>{(*Reader), pheight_name.c_str()};
    };

    if (lanltree) {
        for (unsigned it = 0; it < hpgeChans.size(); ++it) {
            hpge.at(it).detNum = it;
            hpge.at(it).locNum = hpgeChans.at(it);
            string eslow_name = "eslow_" + to_string(hpgeChans.at(it));
            hpge.at(it).eslow = new TTreeReaderValue<Double_t>{(*Reader), eslow_name.c_str()};
            string efast_name = "efast_" + to_string(hpgeChans.at(it));
            hpge.at(it).efast = new TTreeReaderValue<Double_t>{(*Reader), efast_name.c_str()};
            string twrap_name = "twrap_" + to_string(hpgeChans.at(it));
            hpge.at(it).twrap = new TTreeReaderValue<Double_t>{(*Reader), twrap_name.c_str()};
            string pheight_name = "pheight_" + to_string(hpgeChans.at(it));
            hpge.at(it).pheight = new TTreeReaderValue<Double_t>{(*Reader), pheight_name.c_str()};
        }

        for (unsigned it = 0; it < siChans.size(); ++it) {
            si.at(it).detNum = it;
            si.at(it).locNum = siChans.at(it);
            string eslow_name = "eslow_" + to_string(siChans.at(it));
            si.at(it).eslow = new TTreeReaderValue<Double_t>{(*Reader), eslow_name.c_str()};
            string efast_name = "efast_" + to_string(siChans.at(it));
            si.at(it).efast = new TTreeReaderValue<Double_t>{(*Reader), efast_name.c_str()};
            string twrap_name = "twrap_" + to_string(siChans.at(it));
            si.at(it).twrap = new TTreeReaderValue<Double_t>{(*Reader), twrap_name.c_str()};
            string pheight_name = "pheight_" + to_string(siChans.at(it));
            si.at(it).pheight = new TTreeReaderValue<Double_t>{(*Reader), pheight_name.c_str()};
        }
    }
};

int DetCheckHistos::PlotNeutronSuperPlots(string type2Plot, string subplot2plot, TCanvas* SuperPlot) {
    if (!DataProcessed) {
        cout << " YOU MUST PROCESS THE DATA BEFORE PLOTTING........................................ dingus" << endl;
        return 1;
    };

    if (SuperPlot == nullptr) {
        cout << " You pass a NULLPTR for the TCanvas. Exiting" << endl;
    }

    TObjArray* plots;
    pair<int, int> subDivide_xy;  // number of x spots by num of y spots
    if (strcmp(type2Plot.c_str(), "odesa") == 0) {
        plots = &odesaPlots;
        subDivide_xy = {5, 2};
    } else if (strcmp(type2Plot.c_str(), "stilbene") == 0) {
        plots = &stilbenePlots;
        subDivide_xy = {4, 2};
    } else {
        cout << "This is for Plotting neutron detector plots only. Draw the others manually" << endl;
        return 2;
    }
    int totalNumSlots = subDivide_xy.first * subDivide_xy.second;
    cout << "getting to setlog" << endl;
    SuperPlot->SetLogz();
    SuperPlot->Divide(subDivide_xy.first, subDivide_xy.second);
    int subPadCounter = 1;  // subpads start at 1, cd(0) returns the master pad which isnt fully functional after TPad::Divide()
    for (auto curPlot = plots->begin(); curPlot != plots->end(); ++curPlot) {
        string curPlot_name((*curPlot)->GetName());
        string curSubPlot_name = curPlot_name.substr(curPlot_name.length() - 3);
        if (strcmp(curSubPlot_name.c_str(), subplot2plot.c_str()) == 0) {
            SuperPlot->cd(subPadCounter);
            (*curPlot)->Draw("colz");
            subPadCounter++;
        }
    }
    return 0;
}

int DetCheckHistos::BuildNeutronGatesVector(string dettype, vector<detInfo>& detInfoVec) {
    if (strcmp(dettype.c_str(), "odesa") == 0) {
        //! Hard coding the info here, but one could easily rewrite it to pass in a cfg file.
        vector<double> odesa_psd_eslow_thresh(10, 300);
        vector<double> odesa_psd_low = {0.878, 0.1, 0.1, 0.1, 0.0865, 0.0865, 0.068, 0.0878, 0.1, 0.1};
        vector<double> odesa_psd_high(10, 0.5);
        vector<double> odesa_pheight_clip(10, 150000);
        vector<double> odesa_eslow_cal = {7.7071, 3.7804, 3.5857, 3.8845, 3.6858, 4.0051, 3.7446, 4.4482, 4.1086, 3.980};

        if (odesa_psd_eslow_thresh.size() != odesaChans.size() ||
            odesa_psd_low.size() != odesaChans.size() ||
            odesa_psd_high.size() != odesaChans.size() ||
            odesa_pheight_clip.size() != odesaChans.size() ||
            odesa_eslow_cal.size() != odesaChans.size()) {
            cout << "one of the calibrations didnt load right or is missing one." << endl;
            return 2;
        }
        detInfoVec = vector<detInfo>(odesaChans.size(), detInfo());
        for (unsigned iter = 0; iter < odesaChans.size(); ++iter) {
            detInfoVec.at(iter).eslow_cal_m = odesa_psd_eslow_thresh.at(iter);
            detInfoVec.at(iter).psd_high = odesa_psd_high.at(iter);
            detInfoVec.at(iter).psd_low = odesa_psd_low.at(iter);
            detInfoVec.at(iter).psd_eslow_thresh = odesa_psd_eslow_thresh.at(iter);
            detInfoVec.at(iter).pheight_clip = odesa_pheight_clip.at(iter);
        }

        odesa_has_a_neutron = vector<bool>(odesaChans.size(), false);
        return 0;
    } else {
        return 1;
    }
}

int DetCheckHistos::Make_Mike_Histos(vector<int> Chans, string dettype, TObjArray& objarray) {
    if (strcmp(dettype.c_str(), "odesa") == 0) {
        string detPrefix = dettype.substr(0, 3);
        string detSuffix = "_mike";
        for (unsigned chanIT = 0; chanIT < Chans.size(); ++chanIT) {
            string MikeTof_NGated = detPrefix + to_string(Chans.at(chanIT)) + "_tofvE" + detSuffix;
            string PSD1_title = detPrefix + to_string(Chans.at(chanIT)) + " N Cuts (Mike style)";
            objarray.Add(new TH2D(MikeTof_NGated.c_str(), PSD1_title.c_str(), 1000, 0, 200, 1000, 0, 1000));
        }
    } else if (strcmp(dettype.c_str(), "hpge") == 0) {
        for (unsigned chanIT = 0; chanIT < Chans.size(); ++chanIT) {
            for (unsigned chanIT2 = 0; chanIT2 < odesaChans.size(); ++chanIT2) {
                string detprefix = "hpge";
                string detSuffix = "_mike";
                string hpge_key = detprefix + to_string(Chans.at(chanIT)) + "_e_ode" + to_string(odesaChans.at(chanIT2)) + "_ncut_mike";
                string hpge_title = detprefix + " " + to_string(Chans.at(chanIT)) + " eSlow with Neutron in OdesaChan # " + to_string(odesaChans.at(chanIT2)) + " (Mike style)";

                objarray.Add(new TH1D(hpge_key.c_str(), hpge_title.c_str(), 5000, 0, 5000));
            }
        }
    } else {
        return 1;
    }
    return 0;
}
