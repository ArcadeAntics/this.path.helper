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


SEXP do_getconnection(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    Rconnection Rcon = R_GetConnection(CADR(args));
    return R_MakeExternalPtr((void *) Rcon, R_NilValue, R_NilValue);
}


typedef struct gzconn {
    Rconnection con;
} *Rgzconn;


SEXP do_getunderlyingconnection(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    Rconnection Rcon = R_GetConnection(CADR(args));
    if (Rcon->isGzcon) {
        /* copied from
https://github.com/wch/r-source/blob/50ff41b742a1ac655314be5e25897a12d3096661/src/main/connections.c#L6018
         * this gives us access to the original connection that the gzcon was derived from
         */
        Rcon = (((Rgzconn)(Rcon->private))->con);
    }
    return R_MakeExternalPtr((void *) Rcon, R_NilValue, R_NilValue);
}





static const R_CallMethodDef callRoutines[] = {

    {NULL, NULL, 0}
};


static const R_ExternalMethodDef externalRoutines[] = {

    {"getconnection"          , (DL_FUNC) &do_getconnection          , 1},
    {"getunderlyingconnection", (DL_FUNC) &do_getunderlyingconnection, 1},

    {NULL, NULL, 0}
};


void attribute_visible R_init_this_path_helper(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, callRoutines, NULL, externalRoutines);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}
