omegahatReference <-
function(key, named=T)
{
 ans <- list(key=key, className="")
 class(ans) <- ifelse(named, "NamedOmegahatReference", "AnonymousOmegahatReference")

 ans
}
