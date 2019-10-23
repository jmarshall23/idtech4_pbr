@echo off
del post_cas.vfp
cgc  -profile arbvp1 cgvp_cas.cg
echo #====================================================================== >> post_cas.vfp
cgc -profile arbfp1 cgfp_cas.cg

pause