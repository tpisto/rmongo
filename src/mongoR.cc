#include "client/dbclient.h"
#include <iostream>
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define MONGO_ERROR(a) { error(a); fputs(a,stdout);exit(EXIT_FAILURE);}

#ifndef assert
#  define assert(x) MONGO_assert(x)
#endif

using namespace std;
using namespace mongo;

extern "C" {
	
/**
 * connect to MONGO
 */
SEXP mongoRconnect(SEXP host, SEXP port)
{
    const char *myPort = CHAR(STRING_ELT(port, 0));
	
    DBClientConnection *conn = new DBClientConnection();
    string errmsg;
    if ( ! conn->connect( string( CHAR(STRING_ELT(host, 0)) ) + ":" + myPort , errmsg ) ) {
        cout << "couldn't connect : " << errmsg << endl;
        exit(EXIT_FAILURE);
    }

	/// the handle is bound a R variable 
    return R_MakeExternalPtr(conn, R_NilValue, R_NilValue);
}

//
// authenticate to MONGO
//
SEXP mongoRauthenticate(SEXP dbname, SEXP user, SEXP password) 
{
	return R_NilValue;
}

//
// close the mongo connection
// 
SEXP mongoRdisconnect(SEXP conn_handle)
{
	DBClientConnection *conn = (DBClientConnection *)R_ExternalPtrAddr(conn_handle);
	if(conn==NULL) MONGO_ERROR("conn==NULL");
	delete conn;
	R_ClearExternalPtr(conn_handle);
	return ScalarInteger(0);	
}

//
// Query
//
SEXP mongoRquery(SEXP conn_handle, SEXP collection, SEXP query)
{
	SEXP resultJson=NULL;
	int i;
	SEXP *array=NULL;
	int array_size=0;
	
	DBClientConnection *conn = (DBClientConnection *)R_ExternalPtrAddr(conn_handle);
	if(conn==NULL) MONGO_ERROR("conn==NULL");
	
	BSONObj myQuery = mongo::fromjson( string(CHAR(STRING_ELT(query, 0))) );  
	auto_ptr<DBClientCursor> cursor = conn->query(CHAR(STRING_ELT(collection, 0)), myQuery);
	
	while( cursor->more() ) {
		array=(SEXP*)realloc(array,(array_size+1)*sizeof(SEXP));
		if(array==NULL) error("out of memory");
		array[array_size]=mkChar( cursor->next().toString().c_str() );
		array_size++;
	}
	
	PROTECT(resultJson = allocVector(STRSXP, array_size));
	for(int i=0;i< array_size;++i)
    {
		SET_STRING_ELT(resultJson, i, array[i]);
    }
	free(array);
	UNPROTECT(1);

	return resultJson; 
}

	
} // extern "C"
