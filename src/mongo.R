dyn.load(paste("librmongo", .Platform$dynlib.ext, sep=""))

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

mongo <- mongo.open()
mongo.query(mongo, "test.blog_post", "{}")
mongo.close(mongo)