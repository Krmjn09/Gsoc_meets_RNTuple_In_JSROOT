//CALLING ANOTHER MACRO example but its not present so it gives error
void cernstaff() {
    if (gSystem->AccessPathName("cernstaff.root")) {
       int errorCode = gROOT->ProcessLine(".x cernbuild.C");
    }
 }


