#ifndef __UTILITY_H
#define __UTILITY_H

#include <wx/wx.h>

#define START_TIMER wxStartTimer();
#define LOOP for (int count=0 ; count<1000000 ; count++)
#define END_TIMER I(wxGetElapsedTime());

#define MEM_CHECK(result)            \
   if ((result) == NULL)             \
   { Text_Error("Out of Memory");    \
     exit(0);                        \
   }

void Text_Error(wxString message);
void File_Error(wxString filename);
void Num_Error(long num);

void I(long num);
void F(double num);

int Random(int low, int high);
void Bubble_Sort(double data[], int elem);

wxString Bool_Str(unsigned char bool_var);


#endif // __UTILITY_H
