setwd("bin") 
source("mongo.R");

mongo <- mongo.open()
mongo.insert(mongo, "test.urlrouter_urlroute", '{ author: "joe", created_date: new Date(234234234234) }')
mongo.query(mongo, "test.urlrouter_urlroute", '{}')
mongo.close(mongo)
