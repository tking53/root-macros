#ifndef TObjArrayWriter_cxx
#define TObjArrayWriter_cxx
#include <TROOT.h>
#include <TObjArray.h>

void WriteTObjArray(TObjArray* iArray){
    TIter next(iArray);
    while (TObject *obj = next()) {
        obj->Write();
    }
}
#endif