#
# This is similar to dataFrameView but rather than copying
# the contents of the R data frame to a corresponding Java 
# class that is self-contained, we send a reference to the 
# R data frame. 
#
# Currently there is a problem with synchronization
# in threads that callback to R.  So instead of 
# displaying the data frame in a Java GUI which
# runs in a separate thread, we pass it as a 
# an argument to a test class that exercises
# the different methods



 # a reduced version of the Java interface
 # Not all the methods of DataFrameInt are implemented,
 # just the ones we need. This is the advantage of 
 # mixing weak- and strongly-typed languages (R and Java). 

dataFrameHandler <- function(data)
{

  exportedVars <- list()

  getName <- function(){"mtcars"}
  numObservations <- function() {nrow(data)} 
  numVariables <- function() { ncol(data) }
  getVariableNames <- function() {colnames(data)}
  getVariable <- function(varName) { 
    if(is.na(match(varName,names(exportedVars)))) {
      tmp <- .JavaConstructor("RealVariable",as.numeric(data[,varName]))
       exportedVars[[varName]] <- tmp
    }
     exportedVars[[varName]]
  } 

  rowNames <- function() { rownames(data) }

  list(rowNames = rowNames,
       numObservations = numObservations,
       numVariables = numVariables,
       getVariableNames = getVariableNames,
       getVariable = getVariable,
       getName = getName)
}

data(mtcars)
ref <- foreignReference(dataFrameHandler(mtcars))


compiler <- .JavaConstructor("ForeignReferenceClassGenerator", "org.omegahat.DataStructures.Data.DataFrameInt", "RDynamicDataFrame")
dyn <- .Java("__Evaluator","dynamicClassLoader")
.Java(dyn,"defineClass", compiler)


jdata <- .JavaConstructor("RDynamicDataFrame",  ref)

#
.JavaConstructor("TestDataFrame", jdata)

# cat("Creating viewer\n")
# w <- .JavaConstructor("DataFrameViewerWindow", jdata)

