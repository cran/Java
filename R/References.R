getForeignReferences <-
function(which = c(named=T, anonymous=F))
{
 ans <- vector("list", length(which))

 for(i in 1:length(which)) {
  ans[[i]] <- .Java(NULL, "getReferences", as.logical(which[i]))
 }

 if(length(ans) == length(which))
   names(ans) <- names(which)

 return(ans)
}
