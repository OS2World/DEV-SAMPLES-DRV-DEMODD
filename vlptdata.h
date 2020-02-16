/*static char *SCCSID = "@(#)vlptdata.h 6.7 91/08/23";*/
#pragma linesize(132)
#pragma pagesize(60)
#pragma title("VLPTDATA.H")
/****************************************************************************/
/*                                                                          */
/*                       IBM/Microsoft Confidential                         */
/*                                                                          */
/*                 Copyright (c) IBM Corporation  1987, 1990                */
/*                 Copyright (c) Microsoft Corp.  1987, 1990                */
/*                           All Rights Reserved                            */
/*                                                                          */
/****************************************************************************/
/************************** START OF SPECIFICATIONS ***************************/
/*                                                                            */
/*   SOURCE FILE NAME:  VLPTDATA.H                                            */
/*                                                                            */
/*   DESCRIPTIVE NAME:  VLPT external data                                    */
/*                                                                            */
/*   FUNCTION: This routine sets up the externs to match the instance data    */
/*             area and the global data area for VLPT.                        */
/*                                                                            */
/*   NOTES:                                                                   */
/*      DEPENDENCIES: Any data structure changes made to VLPTDATA.C must      */
/*                    also be changed in this file. VLPTDATA.C contains       */
/*                    the data that this file makes into external data.       */
/*      RESTRICTIONS: None                                                    */
/*                                                                            */
/*   ENTRY POINTS:                                                            */
/*             None                                                           */
/*                                                                            */
/*   EXTERNAL REFERENCES:                                                     */
/*             None                                                           */
/*                                                                            */
/**************************** END OF SPECIFICATIONS ***************************/

#pragma BEGIN_SWAP_INSTANCE

extern struct instance_data pvd_area[];    /* per VDM data area            */
extern HVDM this_VDM;                      /*  actual VDM handle           */
extern UINT resp_val;                      /*  response value from popup   */
extern HVDHSEM vlpt_sem_handle;            /*  semaphore handle for delays */
extern PCHAR lptname;                      /* device name prefix           */
extern PCHAR part_disk;
extern PCHAR disk_string;
extern BOOL io_installed[];                /* IO handlers installed        */
extern HHOOK hhookVLPTIRet5;               /* handle for VDHArmReturnHook  */
extern VLPTPDB pdbCurrent;                 /* current PDB,Process Data Blk.*/
extern ULONG nmsLptTimeout;                /* per vdm lpt starting value   */

#pragma END_SWAP_INSTANCE

#pragma BEGIN_SWAP_DATA

// test
extern struct VDHMapSource_s MapSource;
extern struct VDHMapTarget_s MapTarget;
// test

extern ULONG ulbuffersize[];                    /* Size of print data bffr */
extern HVDD hvddvbios;                          /* Addr. of VBIOS entry pt.*/
extern PPOSREGS pvddvbiospos;                   /* Address of POS Regs     */
extern ULONG MachineType;                       /* Machine Type            */
extern FPFNPDD PLPTPDDProc;                     /* addr of PDD entry point */
extern struct port_address vpa_table[];         /* VLPT port address table */
extern struct def_bios_data_area def_bd_area[]; /* Default bios data       */
extern struct error_table_record other_table[];  /* OTHER error table      */
extern struct error_table_record direct_table[]; /* DIRECT error table     */
extern struct vlpt_ioh_table_s v_ioh[];          /*   LPT IO handler table */

#pragma END_SWAP_DATA
