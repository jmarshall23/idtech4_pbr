@echo off
del basicoccluder.vfp
cgc -quiet -profile arbvp1 cgvp_basicoccluder.cg >> basicoccluder.vfp
echo #====================================================================== >> basicoccluder.vfp
cgc -quiet -profile arbfp1 cgfp_basicoccluder.cg >> basicoccluder.vfp
