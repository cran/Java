.JavaArrayConstructor <- 
#
# 
#
function(klass, ..., dim=length(list(...)), .name=NULL, .convert=F)
{
  args <- list(...)
  if(missing(dim)) {
    dim <- length(args)
  } else
    dim <- as.integer(dim)

 arr <- .Java(NULL, "createArray", klass, dim, .name=.name, .convert=.convert)
 # Now set the values in ...
 #  .Call("RS_Java_ArrayConstructor", as.character(klass), dim)

 arr
}


.JavaGetArrayElement <-
function(jobj, ..., .name=NULL, .convert=T)
{
  index <- sapply(list(...), function(x) as.integer(x))
  .Java(NULL, "getArrayElement", jobj, as.integer(index - 1), .name=.name, .convert=.convert);
}

.JavaSetArrayElement <-
function(jobj, value, ...)
{
  index <- sapply(list(...), function(x) as.integer(x))
  .Java(NULL, "setArrayElement", jobj, as.integer(index - 1), value);
}

.JavaArrayLength <-
function(jobj)
{
 .Java(NULL, "arrayLength", jobj)
}
