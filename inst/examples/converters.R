 val <- setJavaConverter(.RSJava.symbol("RealVariableConverter"), matcher="AssignableFrom",
                          autoArray=T, description="Omegahat RealVariable to numeric vector",
                           userObject="RealVariable"
                         )

   # now instantiate an object of class RealVariable
   # and the converter should take effect.
   # should produce a numeric vector of length 10
 .JavaConstructor("RealVariable", rnorm(10))


   # Un-register the converter.
  removeJavaConverter(val$index)
