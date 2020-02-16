/*----------------------------------------------------------------------------
     Modul     :    DEMODDEX.H

     Meaning   :    External interface for DOS-Apps using the VDD. The FUNC_XXX
                    values have to be used in the AH register as a code.

     Autor     :    Michael Glienecke

     History   :    23.02.1993
*----------------------------------------------------------------------------*/
#define   DEMOINT        0x90           // INT for VDM

#define   FUNC_OPENPDD   0x01           // open the PDD
#define   FUNC_CLOSEPDD  0x02           // close the PDD
#define   FUNC_SUBFUNC1  0x03           // IOCtl 1
#define   FUNC_SUBFUNC2  0x04           // IOCtl 2
#define   FUNC_SUBFUNC3  0x05           // IOCtl 3
