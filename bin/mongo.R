dyn.load(paste("librmongo-darwin-64", .Platform$dynlib.ext, sep=""))

mongo.open <- function(host="127.0.0.1", port=27017)
{
.Call("mongoRconnect", as.character(host), as.character(port))
}

mongo.close <- function(handler)
{
.Call("mongoRdisconnect",handler)
}

mongo.query <- function(handler, collection, query)
{
.Call("mongoRquery",handler, as.character(collection), as.character(query))
}

mongo.insert <- function(handler, collection, query)
{
	.Call("mongoRinsert",handler, as.character(collection), as.character(query))
}
