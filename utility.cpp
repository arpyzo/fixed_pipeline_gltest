#include "utility.h"

/********************************** Errors ************************************/

/**************/
/* Text_Error */
/**************/
void Text_Error(wxString message)
{ wxMessageBox(message, "Error!", wxICON_ERROR | wxOK);
}

/**************/
/* File_Error */
/**************/
void File_Error(wxString filename)
{ wxMessageBox("The file: \"" + filename + "\" could not be loaded!", "Error!", wxICON_ERROR | wxOK);
}

/********************************** Debugging *********************************/

/*****/
/* I */
/*****/
void I(long num)
{ wxMessageBox(wxString::Format("%d", num));
}

/*****/
/* F */
/*****/
void F(double num)
{ wxMessageBox(wxString::Format("%f", num));
}

/********************************** Numerical *********************************/

/**********/
/* Random */
/**********/
int Random(int low, int high)
{ int new_max, num;

  if (low >= high)
    return low;

  int range = (high - low) + 1;
  new_max = RAND_MAX / range;
  new_max *= range;
  
  do
  { num = rand();
  } while (num > new_max);

  return num % range + low;
}

/***************/
/* Bubble_Sort */
/***************/
void Bubble_Sort(double data[], int elem)
{ int x, y;
  double temp;
  
  for(x = 1 ; x < elem ; x++)
  { for(y = 0 ; y < x ; y++)
    { if(data[x] < data[y])
      { temp = data[x];
        data[x] = data[y];
        data[y] = temp;
      }
    }
  }
}

/******************************** Miscellaneous *********************************/

/************/
/* Bool_Str */
/************/
wxString Bool_Str(unsigned char bool_var)
{ if (bool_var)
    return "True";
  else
    return "False";
}