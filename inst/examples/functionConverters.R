setJavaFunctionConverter(function(x, className) {
           print("This is a silly converter for a JButton")
           val <- .Java(x,"getText")
           val <- c(val,.Java(x,"getActionCommand"))
           print(val)
           return(val)
    }, function(obj, className){
     ok <- className == "javax.swing.JButton"
     cat("In match:",ok,"\n")
     return(ok)
})

setJavaConvertible("JButton")
print(.JavaConstructor("JButton", "testing"))

# Now we use a closure to handle both the conversion and matching
# and this gives us mutable state.
#
# We just count the number of times the converter is called.
 
realVariableConverterHandler <-
 function() {
   n <- 0
   cvt <- function(obj, className) {
      n <<- n + 1
      .Java(obj, "getValues")
   }
   matcher <- function(obj, className) {
     return(className == "org.omegahat.DataStructures.Data.RealVariable")
   }
   return(list(converter=cvt, matcher=matcher, count = function(){ n } ))
 }


rvCvt <- realVariableConverterHandler()
setJavaFunctionConverter(rvCvt$converter, rvCvt$matcher)

setJavaConvertible("RealVariable")
print(.JavaConstructor("RealVariable", rnorm(10)))




