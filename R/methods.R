
#
# These are convenience methods for 
# accessing (calling) Java methods and fields.
# It uses the $ operator
#
# Obviously we should have AnonymousOmegahatReference
# and NamedOmegahatReference should inherit from a common
# base class and this $ method should be defined for it.

"$.AnonymousOmegahatReference" <-
function(obj,name)
{
 function(...) {
  .Java(obj, as.character(name),...)
 }
}

"$.NamedOmegahatReference" <-
function(obj,name)
{
 function(...) {
  .Java(obj, as.character(name),...)
 }
}
