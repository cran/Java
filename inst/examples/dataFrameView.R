# This copies the contents of the mtcars data frame to a Java class
# (DataFrame) and then displays this using the DataFrameViewer class
# (and its window version).

data(mtcars)
jdata <- .JavaConstructor("DataFrame")
for( i in 1:ncol(mtcars)) 
  .Java(jdata,"addVariable", names(mtcars)[i], .JavaConstructor("RealVariable", mtcars[,i]))

w <- .JavaConstructor("DataFrameViewerWindow", jdata)
