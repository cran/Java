isJavaInitialized <-
function(msg=NULL)
{
  ok <- logical(1)
  ok <- .C("RS_JAVA_isJVMInitialized",ok=ok)$ok
 if(!ok & !missing(msg))
  stop(msg)

 ok
}
