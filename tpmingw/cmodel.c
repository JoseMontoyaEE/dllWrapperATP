extern void  outsix_( char *, int *);
        
    void c_sample_m__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[])
    { short i;
      char* text = "Executing model 'sample_c_model'.";
      int len = strlen(text);
      outsix_(text, &len);
      for (i=0; i<xdata_ar[0]; i++)
      { xvar_ar[i] = (2*xvar_ar[i] + xin_ar[i])/2;
        xout_ar[i] = xvar_ar[i] + 100.0;
      }
      return;
    }
        
    void c_sample_i__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[])
    { char* text = "Initializing model 'sample_c_model'.";
      int len = strlen(text);
      outsix_(text, &len);
      return;
    }
 
 /* Remarks:
 |   xin_ar  holds input values received from the calling model.
 |   xout_ar holds output values sent back to the calling model.
 |   xvar_ar holds state values stored in memory by MODELS between calls.
 |
 |   Note how xvar_ar is referenced before a new value is assigned to it,
 |    requiring a reference to the pre-simulation history of xvar_ar
 |    at the first execution of the procedure.
 |
 |   Note that you need to modify ATP file fgnmod.for in order to connect
 |   additional user-defined foreign subroutines and procedures.
 |
*/
