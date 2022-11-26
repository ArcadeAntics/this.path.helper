#include <R.h>
#include <Rinternals.h>
#include <R_ext/Connections.h>


#include <stdlib.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Visibility.h>





#if !defined(R_CONNECTIONS_VERSION)
    #error why is R_CONNECTIONS_VERSION not defined????
#elif R_CONNECTIONS_VERSION == 1
    extern Rconnection R_GetConnection(SEXP sConn);
#else
#error this.path.helper is only implemented for R_CONNECTIONS_VERSION 1
#endif


extern Rboolean R_Visible;


void set_R_Visible(Rboolean visible)
{
    R_Visible = ((visible) ? (TRUE) : (FALSE));
}


typedef struct gzconn {
    Rconnection con;
} *Rgzconn;


Rconnection R_GetUnderlyingConnection(SEXP file)
{
    Rconnection Rcon = R_GetConnection(file);
    if (Rcon->isGzcon) {
        /* copied from
https://github.com/wch/r-source/blob/50ff41b742a1ac655314be5e25897a12d3096661/src/main/connections.c#L6018
         * this gives us access to the original connection that the gzcon was derived from
         */
        return ((Rgzconn)(Rcon->private))->con;
    }
    else return Rcon;
}





static const R_CallMethodDef callRoutines[] = {
    {NULL, NULL, 0}
};


static const R_ExternalMethodDef externalRoutines[] = {
    {NULL, NULL, 0}
};


void attribute_visible R_init_this_path_helper(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, callRoutines, NULL, externalRoutines);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);


    R_RegisterCCallable("this.path.helper", "set_R_Visible"            , (DL_FUNC) &set_R_Visible          );
    R_RegisterCCallable("this.path.helper", "R_GetUnderlyingConnection", (DL_FUNC) &R_GetUnderlyingConnection);
}
