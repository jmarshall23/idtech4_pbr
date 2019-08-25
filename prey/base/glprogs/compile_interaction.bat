@echo off
del interaction.vfp
cgc -quiet -profile arbvp1 cgvp_interaction.cg >> interaction.vfp
echo #====================================================================== >> interaction.vfp
cgc -quiet -profile arbfp1 cgfp_interaction.cg >> interaction.vfp
copy interaction.vfp interactionAll.vfp
copy interaction.vfp interactionNormBump.vfp
copy interaction.vfp interactionCubeMaps.vfp
copy interaction.vfp interactionUpdate.vfp
copy interaction.vfp interactionMasked.vfp
copy interaction.vfp interactionNormBump.vfp
copy interaction.vfp interactionNormBumpH.vfp
