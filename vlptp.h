/*static char *SCCSID = "%W% %E%";*/
#pragma linesize(132)
#pragma pagesize(60)
#pragma title("VLPTP.H")
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
/*   SOURCE FILE NAME:  VLPTP.H                                               */
/*                                                                            */
/*   DESCRIPTIVE NAME:  VLPT private constant definitions and data structure  */
/*                      definitions                                           */
/*                                                                            */
/*   FUNCTION: This routine sets the constant definitions and data structure  */
/*             definitions used by VLPT.                                      */
/*                                                                            */
/*   NOTES:                                                                   */
/*                                                                            */
/**************************** END OF SPECIFICATIONS ***************************/
                                    /*                                     */
#define INT_17          0x17        /* Print interrupt vector number 17    */
#define INT_05          0x05        /* Print interrupt vector number 5     */
#define MAXPRTS         3           /* Max. number of LPTs supported       */
#define MAXERROR        5           /* Max. number of errors               */
#define LPTNAME      "LPT \0"       /* LPTx string                         */
#define DRIVENUM    " :\0"
#define DISKSTRING  "0123456789"
#define LPTONE          0           /* LPT 1 offset                        */
#define LPTTWO          1           /* LPT 2 offset                        */
#define LPTTHREE        2           /* LPT 3 offset                        */
#define OPEN_TYPE       0           /* doOpen      type for processing     */
#define FLUSH_TYPE      1           /* doFlush     type for processing     */
#define CLOSE_TYPE      2           /* doClose     type for processing     */
#define ALLOC_TYPE      3           /* VDHAllocMem type for processing     */
#define DIRECT_TYPE     4           /* direct mode type for processing     */
#define PORTS         "123"         /* Global valid port ids  1,2,3        */
#define USED            1           /* Port address USED indicator         */
#define UNUSED          0           /* Port address UNUSED indicator       */
#define BUFSIZ          128         /* Size of character buffer            */
#define DEFLPT1         0x03BC      /* Default port address for LPT1       */
#define DEFLPT2         0x0378      /* Default port address for LPT2       */
#define DEFLPT3         0x0278      /* Default port address for LPT3       */
#define ZERO_ADDRESS    0x00        /* port address of zero                */
#define PORT_HOLD       3           /* holding area offset for port info.  */
                                    /*                                     */
                                    /***************************************/
                                    /* Virtual STATUS port bit definitions */
                                    /*                                     */
                                    /*                                     */
                                    /*   bit bit bit bit bit bit bit bit   */
                                    /*    7   6   5   4   3   2   1   0    */
                                    /*  ---------------------------------  */
                                    /*  | 1 | 1 | 0 | 1 | 1 | 1 | 1 | 1 |  */
                                    /*  ---------------------------------  */
                                    /*    |   |   |   |   |   |   |   |    */
                                    /*    v   |   v   |   v   |   v   |    */
                                    /*  BUSY  |  PE   | ERROR |reserv-|    */
                                    /*        |       |       |  ed   |    */
                                    /*        v       v       v       v    */
                                    /*       ACK   SELECT    IRQ  reserved */
                                    /***************************************/
                                    /*                                     */
#define BUSY_BIT        0x80        /* BUSY bit indicator in status field  */
#define POWER_ON_STATUS 0x0DF       /* Power on status port setting        */
#define STATUS_MASK     0x0F8       /* Status port un-used bit mask off    */
#define STATUS_REV      0x048       /* Status port reverse bit mask        */
                                    /*                                     */
                                    /***************************************/
                                    /* Virtual CONTROL port bit definitions*/
                                    /*                                     */
                                    /*                                     */
                                    /*   bit bit bit bit bit bit bit bit   */
                                    /*    7   6   5   4   3   2   1   0    */
                                    /*  ---------------------------------  */
                                    /*  | 1 | 1 | 1 | 0 | 1 | 1 | 0 | 0 |  */
                                    /*  ---------------------------------  */
                                    /*    |   |   |   |   |   |   |   |    */
                                    /*    v   |   v   |   v   |   v   |    */
                                    /*reserved| direc-| SLCT  | AUTO  |    */
                                    /*        | tion  |  IN   | FD XT |    */
                                    /*        v       v       v       v    */
                                    /*     reserved  IRQ    INIT    STROBE */
                                    /*               EN                    */
                                    /***************************************/
                                    /*                                     */
#define CTRL_DIRECTION_BIT 0x020    /* Control port direction bit, bit 5   */
#define CTRL_IN_MASK    0x0C0       /* Control port IN reserved bits mask  */
#define CTRL_OUT_MASK   0x02F       /* Control port OUT reserved bits mask */
                                    /*                                     */
                                    /***************************************/
                                    /*                                     */
#define RESERVE_MASK    0x0FF       /* Return value mask for reserve port  */
#define TIME_OUT        0x01        /* TIME-OUT indicator                  */
                                    /*                                     */
                                    /***************************************/
                                    /* Per VDM Data Area FLAGS definitions */
                                    /*                                     */
                                    /*                                     */
                                    /*   bit bit bit bit bit bit bit bit   */
                                    /*    7   6   5   4   3   2   1   0    */
                                    /*  ---------------------------------  */
                                    /*  | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  */
                                    /*  ---------------------------------  */
                                    /*    |   |   |   |   |   |   |   |    */
                                    /*    v   |   v   |   v   |   v   |    */
                                    /*  open  | flush | open/ | prt-  |    */
                                    /*  error | error | closed| screen|    */
                                    /*        v       v       v       v    */
                                    /*      close   direct  init     not   */
                                    /*      error    mode            used  */
                                    /***************************************/
                                    /*                                     */
#define OPEN_ERROR_FLAG   0x80      /* OPEN  error indicator               */
#define CLOSE_ERROR_FLAG  0x40      /* CLOSE error indicator               */
#define FLUSH_ERROR_FLAG  0x20      /* FLUSH error indicator               */
#define DIRECT_MODE_FLAG  0x10      /* DIRECT MODE indicator               */
#define OPEN_FLAG         0x08      /* OPEN/CLOSE  complete indicator      */
#define INIT_FLAG         0x04      /* INIT  complete indicator            */
#define PRT_SC_IN_PROG    0x02      /* print screen in progress indicator  */
#define RESV1_FLAG        0x01      /* not used                            */
                                    /*                                     */
#define OP_CL_FL_MASK     0x0E0     /* Open, Close, Flush error mask       */
#define FLAG_NOT_SET      0x00      /*  00000000b                          */
                                    /*                                     */
#define RTN_ERR_PGM     0x0001      /*  response value from VDHPopup       */
#define ABORT           0x0002      /*  response value from VDHPopup       */
#define RETRY           0x0004      /*  response value from VDHPopup       */
#define IGNORE          0x0008      /*  response value from VDHPopup       */
                                    /*                                     */
#define POPUP                1      /* TRUE FLAG                           */
#define NO_POPUP             0      /* FALSE FLAG                          */
                                    /*                                     */
#define RETRY_VAL            5      /* number of times to retry on error   */
#define TIMEOUT_VAL       5000      /* how long before timeout during retry*/
                                    /* timeout interpreted in milliseconds */
                                    /*                                     */
#define INVALID_PDB          0      /* initial PDB value                   */
#define INVALID_HANDLE       0      /* initial File Handle                 */
                                    /*                                     */
#define LPT_NAME          "LPT"     /* Physical Printer Device Driver name */
                                    /*                                     */
#define PLPT_REQUEST_EXCLUSIVE  2   /* VDD-PDD function for open direct    */
#define PLPT_RELEASE_EXCLUSIVE  3   /* VDD-PDD function for close direct   */
#define PLPT_GET_BUFFER_SIZE    4   /* VDD-PDD function for get printer    */
                                    /* buffer size                         */

/***LK+ Advance Properties Configurable constants
 */
#define VLPT_SPOOL_MIN      0   /* minimum value for LPT timeout (No timeout) */
#define VLPT_SPOOL_MAX      60*60   /* max value 1 hour (seconds)             */
#define VLPT_SPOOL_STEP     1   /* advanced property step value (1 second)    */
#define VLPT_SPOOL_CLOSE    15  /* default timeout for LPT to close spool     */
                                /* file in seconds                            */
/*end*/
                                    /*                                     */
typedef USHORT VLPTPDB;             /* define PDB type                     */
                                    /*                                     */
                                    /*                                     */
struct def_bios_data_area {         /* Default VDM Bios Data Area          */
       USHORT lptbase;              /* lpt base port address 16 bit        */
       USHORT used;                 /* Used or not                         */
};                                  /*                                     */
                                    /*                                     */
struct port_address       {         /* LPT port addresses                  */
       USHORT data;                 /* data port address      16 bit       */
       USHORT status;               /* status port address                 */
       USHORT control;              /* control port address                */
       USHORT reserve;              /* reserve port address                */
};                                  /*                                     */
                                    /*                                     */
struct instance_data      {         /* instance data                       */
      UCHAR  flags;                 /* control flags                       */
      UCHAR  data;                  /* virtualized contents of data port   */
      UCHAR  status;                /* virtualized contents of status port */
      UCHAR  lptportid;             /* offset into structure (aka portid)  */
      HFILE  fshandle;              /* handle from VDHOpenPRN              */
      USHORT bufcount;              /* buffer byte counter                 */
      USHORT reserve_word;          /* reserved word                       */
      PCHAR  stream;                /* stream buffer pointer               */
      USHORT pdb_spool;             /* pdb of spool IO process             */
      USHORT pdb_direct;            /* pdb of direct IO process            */
      HHOOK  hhookLptEventContext;  // per lpt context handle
} ;                                 /*                                     */
                                    /*                                     */
typedef struct instance_data *PVLPTINST; // pointer to VLPT instance data structure
typedef PVLPTINST *PPVLPTINST;   // pointer to pointer to instance data
                                    /*                                     */
struct error_table_record {         /* error handling table record         */
      ULONG err_code;               /* error number                        */
      ULONG m_err_code;             /* mapped message file error number    */
      INT   popup;                  /* popup 1 = TRUE  0 = FALSE           */
      ULONG valid_res;              /* valid responses                     */
      ULONG def_res;                /* default responses                   */
      UINT  timeout;                /* timeout                             */
      ULONG retry;                  /* retry count                         */
      PSZZ sub_str;                 /* message substitution string         */
      ULONG num_sub_str;            /* # of message substitution strings   */
};                                  /*                                     */
                                    /*                                     */
struct vlpt_ioh_table_s {           /* IO handle table structure           */
      struct ioh_s iohtable;        /* specific IO handle routine          */
};                                  /*                                     */
                                    /*                                     */

#define NOT_BIDI_MODE      0x080    /* Bi-directional flag of Port 102h    */
#define VBIOS_NAME         "VBIOS"  /* VDD Name for IDC with VBIOS         */

/*----------------------------*/
/*         VLPTINIT.C         */
/*----------------------------*/
BOOL EXPENTRY VDDInit(PSZ);
SBOOL VDDENTRY VLPTPDDProc(ULONG,F16PVOID,F16PVOID);

/*----------------------------*/
/*         VLPTUSER.C         */
/*----------------------------*/
BOOL HOOKENTRY VLPTCreate(HVDM);
BOOL HOOKENTRY VLPTTerminate(HVDM);
BOOL HOOKENTRY VLPTPDBChange(HVDM,VLPTPDB);
BOOL HOOKENTRY VLPTPDBDestroy(HVDM,VLPTPDB);
ULONG EXPENTRY VLPTSetSpoolClose(ULONG, HVDM, ULONG, PSZ);
VOID HOOKENTRY VLPTEventContext(PVOID, PCRF);

/*----------------------------*/
/*         VLPTIN.C           */
/*----------------------------*/
BYTE HOOKENTRY VLPT1DaIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT2DaIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT3DaIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT1StIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT2StIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT3StIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT1CoIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT2CoIn(ULONG, PCRF);
BYTE HOOKENTRY VLPT3CoIn(ULONG, PCRF);
BYTE HOOKENTRY VLPTResIn(ULONG, PCRF);

/*----------------------------*/
/*         VLPTOUT.C          */
/*----------------------------*/
VOID HOOKENTRY VLPT1DaOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT2DaOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT3DaOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT1StOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT2StOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT3StOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT1CoOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT2CoOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPT3CoOut(BYTE, ULONG, PCRF);
VOID HOOKENTRY VLPTResOut(BYTE, ULONG, PCRF);

/*----------------------------*/
/*         VLPTI17.C          */
/*----------------------------*/
BOOL HOOKENTRY VLPTInt17(PCRF);
VOID PRIVENTRY VLPTI17Print(PVLPTINST,PCRF);
VOID PRIVENTRY VLPTI17Init(PVLPTINST);
BYTE PRIVENTRY fixup_status(BYTE);
UINT PRIVENTRY chk_offset(UINT);

/*----------------------------*/
/*         VLPTI05.C          */
/*----------------------------*/
BOOL HOOKENTRY VLPTInt05(PCRF);
VOID HOOKENTRY VLPTIRet5(PVOID,PCRF);

/*----------------------------*/
/*         VLPTDO.C           */
/*----------------------------*/
VOID PRIVENTRY doCheck(PVLPTINST);
VOID PRIVENTRY doOpenSpool(PVLPTINST);
VOID PRIVENTRY doWrite(PVLPTINST);
VOID PRIVENTRY doFlush(PVLPTINST);
VOID PRIVENTRY doCloseSpool(PVLPTINST);
BOOL PRIVENTRY doOpenDirect(PVLPTINST);
VOID PRIVENTRY doCloseDirect(PVLPTINST);
BOOL PRIVENTRY get_direct_access(PVLPTINST);

/*----------------------------*/
/*         VLPTLO.C           */
/*----------------------------*/
UINT PRIVENTRY dopopup(PVLPTINST,struct error_table_record *,UINT);
VOID PRIVENTRY reseterrors(PVLPTINST,UINT);
VOID PRIVENTRY seterrors(PVLPTINST,UINT);
UINT PRIVENTRY proc_error(PVLPTINST,struct error_table_record *,UINT);
