@echo off
del post_newpost.vfp
cgc -quiet -profile arbvp1 cgvp_newpost.cg >> post_newpost.vfp
echo #====================================================================== >> post_newpost.vfp
cgc -quiet -profile arbfp1 cgfp_newpost.cg >> post_newpost.vfp
