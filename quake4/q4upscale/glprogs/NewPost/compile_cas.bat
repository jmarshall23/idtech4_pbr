@echo off
del post_cas_1280.vfp
cgc -quiet -profile arbvp1 cgvp_cas.cg >> post_cas_1280.vfp
echo #====================================================================== >> post_cas_1280.vfp
cgc -quiet -profile arbfp1 cgfp_cas_1280.cg >> post_cas_1280.vfp

del post_cas_1024.vfp
cgc -quiet -profile arbvp1 cgvp_cas.cg >> post_cas_1024.vfp
echo #====================================================================== >> post_cas_1024.vfp
cgc -quiet -profile arbfp1 cgfp_cas_1024.cg >> post_cas_1024.vfp

