setJavaHandler <-
function(handler)
{
   .Call("RJava_setDefaultHandlerFunction", handler)
}

getJavaHandler <-
function()
{
   .Call("RJava_defaultHandlerFunction")
}
