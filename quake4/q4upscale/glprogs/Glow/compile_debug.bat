@echo off
del post_newpost.vfp
cgc  -profile arbvp1 cgvp_basicoccluder.cg
echo #====================================================================== >> post_newpost.vfp
cgc -profile arbfp1 cgfp_basicoccluder.cg

pause