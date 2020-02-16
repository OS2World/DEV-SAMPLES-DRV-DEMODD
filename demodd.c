/*----------------------------------------------------------------------------
     Modul     :    DEMODD.C

     Meaning   :    This is a simple VDD for usage with a DOS-Box under 2.0
                    It is an interface for a DOS-App to communicate with the
                    DIGIO$ device using a familar (for DOS) interface with INTs.
                    This has to be done, because the INT 21h interface in OS/2
                    is lacking the full support for generic IOCtl from DOS to
                    an OS/2 PDD.
                    The interrupt in this driver should be available under all
                    DOS-environments.
                    All buffers, which are used, are taken from the application.
                    The general communication interface is as followed :

                         AH = Function code
                         AL = Subfunction
                         DS:SI = Segment:Offset of a buffer (if needed)
                         CX = length of buffer and length of requested data

                    On exit AX = 0 -> everything is ok, otherwise its the error-
                    code.

     Autor     :    Michael Glienecke

     History   :    23.02.1993

     !!!!!!!!!!!    11.04.1993     A special version for communication with an
                                   OS/2 app using DosRequestVDD was implemented
*----------------------------------------------------------------------------*/
#define   VDDDEBUG                      // We want PRINTDEBUG and printf()

#include  "mvdm.h"                      // Globale Header for VDM

#include  "demodd.h"                    // Lokal Header for the VDD
#include  "demoddex.h"                  // External Header for Apps (DOS)

#define   DIGIO_CATEGORY 0x91           // Category for DIGIO$

#pragma data_seg(CINIT_DATA)            // Init-Data
char szInitMessage[] = "DIGIO-VDD has been initialized\n";
char szDriverName[] = "DEMODD";

#pragma data_seg(CSWAP_DATA)            // Swappable-Data
char szMessage1[] = "DIGIO-VDD for the DOS-Environment activ\n";
char szMessage2[] = "DOS-Environment built using Hook\n";
char szMessage3[] = "INT-Handler has been called\n";
char szMessage4[] = "DOS-Environment left using Hook\n";
char szMessage5[] = "OS2ReqFunc entered\n";
char szReqTRUE[]  = "Request left with TRUE\n";
char szReqFALSE[] = "Request left with FALSE\n";
char szFunc1[]    = "FUNC_OPENPDD\n";
char szFunc2[]    = "FUNC_CLOSEPDD\n";
char szFunc3[]    = "FUNC_SUBFUNC1\n";
char szFunc4[]    = "FUNC_SUBFUNC2\n";
char szFunc5[]    = "FUNC_SUBFUNC3\n";

char szDigioPDD[] = "DIGIO$";           // Name of the device

ULONG     ulActionTaken;                // Beschreibung der verwendeten Aktion

/*------------------------------------------------*/
/* This data-area is kept on a per-VDM-basis      */
/*------------------------------------------------*/
#pragma   data_seg(MVDMINSTDATA)        // VDM-Instancedata
HVDM      CurrVDM;                      // Current VDM
HFILE     hDigio;                       // Handle for Device
BOOL      fOpened;                      // Flag, whether channel has been opened

PVOID     pParameter;                   // converted parameter-adress
PVOID     pBuffer;                      // converted buffer-address
ULONG     ulBytesPassedParam;           // # of parameter-bytes
ULONG     ulBytesPassedBuffer;          // # of buffer-bytes
ULONG     ulLenMaxParam;                // maximal # of parameter-bytes
ULONG     ulLenMaxBuffer;               // maximal # of buffer-bytes

/*------------------------------------------------*/
/* The following code will be arranged automati-  */
/* caly in a non-pageable, non-swappable segment  */
/*------------------------------------------------*/

/*----------------------------------------------------------------------------
     Function  :    DemoDosCreated()    - VDM has been created

     Parameter :    HVDM      hVdm      - Handle of the VDM

     Meaning   :    This function gets called if a VDM is installed. Now we are
                    setting up a new INT-Handler for DEMOINT. This has to be
                    done for every DOS-Box (VDM).
                    From now on, every DEMOINT will be passed directly to us.

     Returns   :    1    -> everything ok
                    0    -> error (VDM will be terminated)
*----------------------------------------------------------------------------*/
BOOL HOOKENTRY DemoDosCreated(HVDM hVdm)
{
     PRINTDEBUG(szMessage1);

     /*------------------------------------------------*/
     /* install INT-Handler for VDM                    */
     /*------------------------------------------------*/
     if ( ! VDHInstallIntHook(0, DEMOINT, DemoIntHandler, 0) ) {
          return(0);
     }

     /*------------------------------------------------*/
     /* some other intialization                       */
     /*------------------------------------------------*/
     CurrVDM = hVdm;                    // set the current VDM for later use
     fOpened = FALSE;                   // no, we have nothing opened

     PRINTDEBUG(szMessage2);
}

/*----------------------------------------------------------------------------
     Function  :    DemoIntHandler()    - Callback fr DEMOINT aus VDM heraus

     Parameter :    PCRF      pRegister - Zeiger auf VDM-Registersatz

     Meaning   :    Nachdem im VDM der DEMOINT aufgerufen worden ist, wird diese
                    Funktion vom VDM angesprungen. Dabei wird als Parameter der
                    komplette Registersatz des VDM bergeben. Dieser kann dann
                    bei Bedarf modifiziert werden.
                    Die Funktion stellt dann entsprechend AH eine Verbindung zum
                    PDD her, oder fhrt IOCTL,s ... durch. Die entsprechenden
                    Codes und Parameter sind in PARAMDEF.DOC nachzulesen.
                    Am Ende werden die Returnwerte des Aufrufes bergeben. Die
                    Funktion kehrt immer mit TRUE zurck, da der INT nicht an
                    den n„chsten VDD gechaint werden soll. Des weiteren wird
                    noch mittels VDHPopInt der INT-Stackframe wieder abgebaut.
                    In AX wird dem Aufrufer mitgeteilt, was passiert ist. AX = 1
                    signalisiert alles ok, AX = 0 -> Fehler aufgetreten, Fehler-
                    code in BX.

     Returns   :    immer TRUE
*----------------------------------------------------------------------------*/
BOOL HOOKENTRY DemoIntHandler(PCRF pRegister)
{
     BOOL      fRetval;

     // INT3();                            // INT 3 durchfhren
     PRINTDEBUG(szMessage3);

     /*------------------------------------------------*/
     /* Von vorneherein die zu verwendenden Parameter  */
     /* auf die sinnvollen Werte setzen. Dies ist viel-*/
     /* leicht Zeitverlust, aber es erleichtert das    */
     /* weitere Programm. Als Konvention gilt immer :  */
     /* DS:SI = Pufferadresse, CX = L„nge              */
     /*------------------------------------------------*/
     pParameter = CalcLinearAdress(DS(pRegister), SI(pRegister));
     ulBytesPassedParam = (ULONG) CX(pRegister);
     ulLenMaxParam = (ULONG) CX(pRegister);

     pBuffer = CalcLinearAdress(DS(pRegister), SI(pRegister));
     ulBytesPassedBuffer = (ULONG) CX(pRegister);
     ulLenMaxBuffer = (ULONG) CX(pRegister);


     /*------------------------------------------------*/
     /* Branch depending on the value of AH            */
     /*------------------------------------------------*/
     switch ( AH(pRegister) ) {
     case FUNC_OPENPDD:                 // open the PDD
          PRINTDEBUG(szFunc1);
          if ( fOpened ) {              // if already open
               fRetval = FALSE;         // its an error
          }
          fRetval = VDHOpen(szDigioPDD, &hDigio, &ulActionTaken,
                       0, VDHOPEN_FILE_NORMAL, VDHOPEN_FILE_OPEN,
                       VDHOPEN_FLAGS_FAIL_ON_ERROR | VDHOPEN_SHARE_DENYNONE, 0);

          if ( fRetval ) {              // if everything is ok
               fOpened = TRUE;          // we are open
          }
          break;

     case FUNC_CLOSEPDD:                // Schlieáen des PDD durchfhren
          PRINTDEBUG(szFunc2);
          fRetval = FALSE;              // Per Default Fehler eintragen
          if ( ! fOpened ) {            // if we didn't open
               break;                   // indicate error
          }
          VDHClose(hDigio);             // close the device
          fOpened = FALSE;              // No longer open
          fRetval = TRUE;               // its ok now
          break;


          /*------------------------------------------------*/
          /* DANGER !!!!! DANGER !!!!!! DANGER !!!!!! DANGER*/
          /*                                                */
          /* If you use a PDD and IOCtls the PDD normally   */
          /* does a LockSeg on the buffers passed. This will*/
          /* fail if the buffer comes from a VDD, because   */
          /* the VMM does a lock itsself, so that the buffer*/
          /* comes to the PDD locked.                       */
          /* Therefore the PDD has to check the typeProcess */
          /* field in the LocalInfoSeg for 1 (DOS-Process)  */
          /* If this is 1, there has to be done no locking  */
          /* and of course no unlocking.                    */
          /* If you block, you have to save this information*/
          /* for instance in the RPstatus-field of your     */
          /* Request-packet. Because this local-info-seg    */
          /* will be unavailable later on.                  */
          /*                                                */
          /* Verify access is fine (no problem)             */
          /*                                                */
          /* DANGER !!!!! DANGER !!!!!! DANGER !!!!!! DANGER*/
          /*------------------------------------------------*/

     case FUNC_SUBFUNC1:                // Subfunction 1 (Write byte to port)
          PRINTDEBUG(szFunc3);
          if ( ! fOpened ) {            // if we are not open
               fRetval = FALSE;         // indicate error
               break;                   // and go out
          }
          fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 1,
                         pParameter, ulLenMaxParam, &ulBytesPassedParam,
                         pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
          break;

     case FUNC_SUBFUNC2:                // Subfunction 2 (Read byte from port)
          PRINTDEBUG(szFunc4);
          if ( ! fOpened ) {            // if we are not open
               fRetval = FALSE;         // indicate error
               break;                   // and go out
          }
          fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 2,
                         pParameter, ulLenMaxParam, &ulBytesPassedParam,
                         pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
          break;

     case FUNC_SUBFUNC3:                // Subfunction 3 (read immediate)
          PRINTDEBUG(szFunc5);
          if ( ! fOpened ) {            // if we are not open
               fRetval = FALSE;         // indicate error
               break;                   // and go out
          }
          fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 3,
                         pParameter, ulLenMaxParam, &ulBytesPassedParam,
                         pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
          break;
     }


     if ( fRetval ) {                   // if everything is ok
          AX(pRegister) = 0;            // set the ok-Flag (0) in AX
          PRINTDEBUG(szReqTRUE);
     } else {
          /*------------------------------------------------*/
          /* Scan for the last error                        */
          /*------------------------------------------------*/
          pRegister->crf_eax = VDHGetError();
          PRINTDEBUG(szReqFALSE);
     }

     VDHPopInt();                       // remove IRET-frame on the stack
     return(TRUE) ;                     // don't chain to the next VDD
}

/*----------------------------------------------------------------------------
     Function  :    CalcLinearAdress()       - computing linear adress

     Parameter :    USHORT    usSeg          - Segment of DOS
                    USHORT    usOff          - Offset of DOS

     Meaning   :    This function computes the linear adress given by Segment:
                    Offset. This is done using the following formula

                    linear adress = Segment << 4 + Offset + Base of VDM

                    The base of the VDM ist the handle of the VDM (passed in
                    to the creation hook). If you look at the typedef this is
                    a PVOID and is the linear starting adress of this DOS-
                    Environment.

     Returns   :    pointer to adress
*----------------------------------------------------------------------------*/
PVOID CalcLinearAdress(USHORT usSeg, USHORT usOff)
{
     ULONG     ulAdress;

     ulAdress = (ULONG) usSeg;
     ulAdress <<= 4;
     ulAdress += (ULONG) usOff;
     ulAdress += (ULONG) CurrVDM;
     return((PVOID) ulAdress);
}

/*----------------------------------------------------------------------------
     Function  :    DemoDosTerminated() - DOS-Session gets terminiert

     Parameter :    HVDM      hVdm      - Handle of the virtual machine

     Meaning   :    This function gets called on termination of a VDM. It does
                    some clean-up if necessary.

     Returns   :    always TRUE
*----------------------------------------------------------------------------*/
BOOL HOOKENTRY DemoDosTerminated(HVDM hVdm)
{

     if ( fOpened ) {                   // if the channel is yet open
          VDHClose(hDigio);             // close it -> PDD likes it
     }

     PRINTDEBUG(szMessage4);            // control-message
     return(TRUE);                      // always the same
}


/*----------------------------------------------------------------------------
     Function  :    OS2ReqFunc          - callback-function for an OS/2 app

     Parameter :    SGID      SGid      - screen group id
                    ULONG     ulCmd     - the wanted command
                    ULONG     cbIn      - count bytes of input data
                    PVOID     pReqIn    - input packet
                    ULONG     cbOut     - count bytes of output data
                    PVOID     pReqOut   - output packet

     Meaning   :    on every DosRequestVDD this function gets called. The
                    parameters are directly passed through.

     Returns   :    TRUE -> Return failure
                    FALSE-> Return success
                    VDDREQ_PASS -> DOS Session Manager calls the next driver
*----------------------------------------------------------------------------*/
LONG EXPENTRY OS2ReqFunc(SGID SGid, ULONG ulCmd, ULONG cbIn, PVOID pReqIn,
               ULONG cbOut, PVOID pReqOut)
{
     // INT3();                         // if you need a break ...

     PRINTDEBUG(szMessage5);            // control-message

     return(VDDREQ_PASS);               // pass on to next VDD in chain
}

/*------------------------------------------------*/
/* The following procedure has to be in the load- */
/* segment, that it can be discarded later on     */
/*------------------------------------------------*/
#pragma alloc_text(CINIT_TEXT, VDDInit) // VDDInit is set in the segment
#pragma entry(VDDInit)                  // VDDInit is the start-func (no main!)

/*----------------------------------------------------------------------------
     Function  :    VDDInit()           - Init-Routine of the VDD

     Parameter :    PSZ  pszConfig      - Pointer to parameters in CONFIG.SYS

     Meaning   :    The function creates on hook for the creation and one hook
                    for the terminaten of VDMs.

     Returns   :    0    -> Error occured
                    1    -> Everything is ok
*----------------------------------------------------------------------------*/
BOOL EXPENTRY VDDInit(PSZ pszConfig)
{
     PRINTDEBUG(szInitMessage);         // Display message

     /*------------------------------------------------*/
     /* Install both hooks for the VDM                 */
     /*------------------------------------------------*/
     if ( ! VDHInstallUserHook(VDM_CREATE, DemoDosCreated) ) {
          return(0);                    // produce error
     }

     if ( ! VDHInstallUserHook(VDM_TERMINATE, DemoDosTerminated) ) {
          return(0);                    // produce error
     }

     /*------------------------------------------------*/
     /* install a hook for DosRequestVDD               */
     /* !!! Brian !!!                                  */
     /*------------------------------------------------*/
     if ( ! VDHRegisterVDD(szDriverName, OS2ReqFunc, NULL) ) {
          return(0);                    // produce an error
     }

     return(1);                         // we are happy
}
