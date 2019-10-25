@echo off
del occluderglow.vfp
cgc -quiet -profile arbvp1 cgvp_glow.cg >> occluderglow.vfp
echo #====================================================================== >> occluderglow.vfp
cgc -quiet -profile arbfp1 cgfp_glow.cg >> occluderglow.vfp
