@echo off
del post_newpost.vfp
cgc  -profile arbvp1 cgvp_newpost.cg
echo #====================================================================== >> post_newpost.vfp
cgc -profile arbfp1 cgfp_newpost.cg

pause