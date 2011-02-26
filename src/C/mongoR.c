#include <ctype.h>
#include <errno.h>
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <bson.h>
#include <mongo.h>

#define MONGO_ERROR(a) { error(a); fputs(a,stdout);exit(EXIT_FAILURE);}


/**
 * connect to MONGO
 */
SEXP mongoRconnect(SEXP host, SEXP port)
{
  mongo_connection* conn; /* ptr */
  mongo_connection_options opts[1];
  mongo_conn_return status;

  conn=(mongo_connection*)malloc(sizeof(mongo_connection));
  if(conn==NULL)
    {
      MONGO_ERROR("out of memory");
    }

  /* Initialize configuration variable) */  
  strcpy( opts->host , CHAR(STRING_ELT(host, 0)) );
  opts->port = INTEGER(port)[0];

  status = mongo_connect( conn, opts );

	// If error, exit. Trying to be verbose about the error
   switch (status) {
        case mongo_conn_bad_arg: MONGO_ERROR( "bad arguments\n" );
        case mongo_conn_no_socket: MONGO_ERROR( "no socket\n" ); 
        case mongo_conn_fail: MONGO_ERROR( "connection failed\n" );
        case mongo_conn_not_master: MONGO_ERROR( "not master\n" ); 
	   case mongo_conn_success: break;
    }

  /** the handle is bound a R variable */
  return R_MakeExternalPtr(conn, R_NilValue, R_NilValue);
}

/**
 * authenticate to MONGO
 */
SEXP mongoRauthenticate(SEXP dbname, SEXP user, SEXP password) 
{
	return R_NilValue;
}

/**
 * close the mongo connection
 */
SEXP mongoRdisconnect(SEXP r_handle)
{
  mongo_connection* conn;
  conn = (mongo_connection*)R_ExternalPtrAddr(r_handle);;
  if(conn==NULL) MONGO_ERROR("conn==NULL");
  mongo_destroy( conn );
  free(conn);
  R_ClearExternalPtr(r_handle);
  return ScalarInteger(0);
}

/**
 * Query
 */
SEXP mongoRquery(SEXP r_handle, SEXP collection, SEXP query)
{
  SEXP values=NULL;
  mongo_cursor *cursor;
  bson bson_query[1];
  mongo_connection* conn = R_ExternalPtrAddr(r_handle);

  bson empty[1];
  bson_empty( empty );	

  // Check that there is valid connection
  if(conn==NULL) MONGO_ERROR("handle==NULL");	

	//bson_init(bson_query, "{ \"moi\" : \"data\" }", 1);
	//bson_print( bson_query );
  // Init JSON query
	// bson_init(bson_query, CHAR(STRING_ELT(query, 0)), 1 );
    cursor = mongo_find( conn, CHAR(STRING_ELT(collection, 0)), empty, empty, 0, 0, 0);		

    while( mongo_cursor_next( cursor ) ) {
        bson_print( &cursor->current );
    }

	/*
	cursor = mongo_find( conn,
		       "test.dbsnps", // ns
		       empty,// fields
		       empty,// return 
		       0,// return 
		       0,// skip 
		       0 // options
		       );
	
  /*
  while( mongo_cursor_next( cursor ) )
    {
      bson_iterator it[1];
      if ( bson_find( it, &(cursor->current), "name" ))
	{
	  array=(SEXP*)realloc(array,(array_size+1)*sizeof(SEXP));
	  if(array==NULL) error("out of memory");
	  array[array_size]=mkChar( bson_iterator_string( it ));
	  array_size++;
	}
    }
  */
	
  // mongo_cursor_destroy( cursor );
  /*
  PROTECT(values = allocVector(STRSXP, array_size));
  for(i=0;i< array_size;++i)
    {
      SET_STRING_ELT(values, i, array[i]);
    }
  free(array);
  UNPROTECT(1);
   */
	
  // return values;
}
