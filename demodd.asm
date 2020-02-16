	TITLE	DEMODD.C
	.386
	.387
	INCLUDELIB os2386.lib
	INCLUDELIB dde4nbs.lib
CODE32	SEGMENT DWORD USE32 PUBLIC 'CODE'
CODE32	ENDS
DATA32	SEGMENT DWORD USE32 PUBLIC 'DATA'
DATA32	ENDS
CONST32	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST32	ENDS
BSS32	SEGMENT DWORD USE32 PUBLIC 'BSS'
BSS32	ENDS
CINIT_TEXT	SEGMENT DWORD USE32 PUBLIC 'CODE'
CINIT_TEXT	ENDS
CINIT_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CINIT_DATA	ENDS
CSWAP_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CSWAP_DATA	ENDS
MVDMINSTDATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
MVDMINSTDATA	ENDS
DGROUP	GROUP CONST32, BSS32, DATA32
	ASSUME	CS:FLAT, DS:FLAT, SS:FLAT, ES:FLAT
	PUBLIC	szInitMessage
	PUBLIC	szDriverName
	PUBLIC	szMessage1
	PUBLIC	szMessage2
	PUBLIC	szMessage3
	PUBLIC	szMessage4
	PUBLIC	szMessage5
	PUBLIC	szReqTRUE
	PUBLIC	szReqFALSE
	PUBLIC	szFunc1
	PUBLIC	szFunc2
	PUBLIC	szFunc3
	PUBLIC	szFunc4
	PUBLIC	szFunc5
	PUBLIC	szDigioPDD
	PUBLIC	ulActionTaken
	PUBLIC	CurrVDM
	PUBLIC	hDigio
	PUBLIC	fOpened
	PUBLIC	pParameter
	PUBLIC	pBuffer
	PUBLIC	ulBytesPassedParam
	PUBLIC	ulBytesPassedBuffer
	PUBLIC	ulLenMaxParam
	PUBLIC	ulLenMaxBuffer
	EXTRN	_printf:PROC
	EXTRN	VDHINSTALLINTHOOK:PROC
	EXTRN	VDHOPEN:PROC
	EXTRN	VDHCLOSE:PROC
	EXTRN	VDHDEVIOCTL:PROC
	EXTRN	VDHGETERROR:PROC
	EXTRN	VDHPOPINT:PROC
	EXTRN	VDHINSTALLUSERHOOK:PROC
	EXTRN	VDHREGISTERVDD:PROC
CINIT_DATA	SEGMENT
szInitMessage	DB "DIGIO-VDD has been initi"
DB "alized",0aH,0H
szDriverName	DB "DEMODD",0H
CINIT_DATA	ENDS
CSWAP_DATA	SEGMENT
	ALIGN 01H
szMessage1	DB "DIGIO-VDD for the DOS-En"
DB "vironment activ",0aH,0H
szMessage2	DB "DOS-Environment built us"
DB "ing Hook",0aH,0H
szMessage3	DB "INT-Handler has been cal"
DB "led",0aH,0H
szMessage4	DB "DOS-Environment left usi"
DB "ng Hook",0aH,0H
szMessage5	DB "OS2ReqFunc entered",0aH,0H
szReqTRUE	DB "Request left with TRUE",0aH,0H
szReqFALSE	DB "Request left with FALSE",0aH
DB 0H
szFunc1	DB "FUNC_OPENPDD",0aH,0H
szFunc2	DB "FUNC_CLOSEPDD",0aH,0H
szFunc3	DB "FUNC_SUBFUNC1",0aH,0H
szFunc4	DB "FUNC_SUBFUNC2",0aH,0H
szFunc5	DB "FUNC_SUBFUNC3",0aH,0H
szDigioPDD	DB "DIGIO$",0H
	ALIGN 04H
ulActionTaken	DB 0H
	DB 04H DUP (00H)
CSWAP_DATA	ENDS
MVDMINSTDATA	SEGMENT
	ALIGN 04H
CurrVDM	DB 0H
	DB 04H DUP (00H)
hDigio	DB 0H
	DB 02H DUP (00H)
	ALIGN 04H
fOpened	DB 0H
	DB 04H DUP (00H)
pParameter	DB 0H
	DB 04H DUP (00H)
pBuffer	DB 0H
	DB 04H DUP (00H)
ulBytesPassedParam	DB 0H
	DB 04H DUP (00H)
ulBytesPassedBuffer	DB 0H
	DB 04H DUP (00H)
ulLenMaxParam	DB 0H
	DB 04H DUP (00H)
ulLenMaxBuffer	DB 0H
	DB 04H DUP (00H)
MVDMINSTDATA	ENDS
DATA32	SEGMENT
DATA32	ENDS
BSS32	SEGMENT
BSS32	ENDS
CONST32	SEGMENT
CONST32	ENDS
CODE32	SEGMENT

;***** 94 BOOL HOOKENTRY DemoDosCreated(HVDM hVdm)
	ALIGN 04H

	PUBLIC DEMODOSCREATED
DEMODOSCREATED	PROC
	PUSH	EBP
	MOV	EBP,ESP

;***** 96      PRINTDEBUG(szMessage1);
	PUSH	OFFSET FLAT:szMessage1
	MOV	AL,01H
	CALL	_printf

;***** 97 
;***** 98      /*------------------------------------------------*/
;***** 99      /* install INT-Handler for VDM                    */
;***** 100      /*------------------------------------------------*/
;***** 101      if ( ! VDHInstallIntHook(0, DEMOINT, DemoIntHandler, 0) ) {
	PUSH	0H
	PUSH	090H
	PUSH	OFFSET FLAT: DEMOINTHANDLER
	PUSH	0H
	CALL	VDHINSTALLINTHOOK
	ADD	ESP,04H
	OR	EAX,EAX
	JNE	FELB1

;***** 102           return(0);
	XOR	EAX,EAX
	JMP	FELB2
	ALIGN 04H
FELB1:

;***** 103      }
;***** 104 
;***** 105      /*------------------------------------------------*/
;***** 106      /* some other intialization                       */
;***** 107      /*------------------------------------------------*/
;***** 108      CurrVDM = hVdm;                    // set the current VDM for later use
	MOV	EAX,[EBP+08H];	hVdm
	MOV	DWORD PTR  CurrVDM,EAX

;***** 109      fOpened = FALSE;                   // no, we have nothing opened
	MOV	DWORD PTR  fOpened,0H

;***** 110 
;***** 111      PRINTDEBUG(szMessage2);
	PUSH	OFFSET FLAT:szMessage2
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 112 }
FELB2:
	LEAVE	
	RET	04H
DEMODOSCREATED	ENDP

;***** 136 BOOL HOOKENTRY DemoIntHandler(PCRF pRegister)
	ALIGN 04H

	PUBLIC DEMOINTHANDLER
DEMOINTHANDLER	PROC
	PUSH	EBP
	MOV	EBP,ESP
	SUB	ESP,04H
	PUSH	ESI
	SUB	ESP,08H

;***** 141      PRINTDEBUG(szMessage3);
	PUSH	OFFSET FLAT:szMessage3
	MOV	AL,01H
	CALL	_printf

;***** 142 
;***** 143      /*------------------------------------------------*/
;***** 144      /* Von vorneherein die zu verwendenden Parameter  */
;***** 145      /* auf die sinnvollen Werte setzen. Dies ist viel-*/
;***** 146      /* leicht Zeitverlust, aber es erleichtert das    */
;***** 147      /* weitere Programm. Als Konvention gilt immer :  */
;***** 148      /* DS:SI = Pufferadresse, CX = L„nge              */
;***** 149      /*------------------------------------------------*/
;***** 150      pParameter = CalcLinearAdress(DS(pRegister), SI(pRegister));
	MOV	EAX,[EBP+08H];	pRegister
	MOV	DX,[EAX+04H]
	MOV	EAX,[EBP+08H];	pRegister
	MOV	AX,[EAX+040H]
	CALL	CalcLinearAdress
	MOV	DWORD PTR  pParameter,EAX

;***** 151      ulBytesPassedParam = (ULONG) CX(pRegister);
	MOV	ECX,[EBP+08H];	pRegister
	XOR	EAX,EAX
	MOV	AX,[ECX+018H]
	MOV	DWORD PTR  ulBytesPassedParam,EAX

;***** 152      ulLenMaxParam = (ULONG) CX(pRegister);
	MOV	ECX,[EBP+08H];	pRegister
	XOR	EAX,EAX
	MOV	AX,[ECX+018H]
	MOV	DWORD PTR  ulLenMaxParam,EAX

;***** 153 
;***** 154      pBuffer = CalcLinearAdress(DS(pRegister), SI(pRegister));
	MOV	EAX,[EBP+08H];	pRegister
	MOV	DX,[EAX+04H]
	MOV	EAX,[EBP+08H];	pRegister
	MOV	AX,[EAX+040H]
	CALL	CalcLinearAdress
	ADD	ESP,04H
	MOV	DWORD PTR  pBuffer,EAX

;***** 155      ulBytesPassedBuffer = (ULONG) CX(pRegister);
	MOV	ECX,[EBP+08H];	pRegister
	XOR	EAX,EAX
	MOV	AX,[ECX+018H]
	MOV	DWORD PTR  ulBytesPassedBuffer,EAX

;***** 156      ulLenMaxBuffer = (ULONG) CX(pRegister);
	MOV	ECX,[EBP+08H];	pRegister
	XOR	EAX,EAX
	MOV	AX,[ECX+018H]
	MOV	DWORD PTR  ulLenMaxBuffer,EAX

;***** 157 
;***** 158 
;***** 159      /*------------------------------------------------*/
;***** 160      /* Branch depending on the value of AH            */
;***** 161      /*------------------------------------------------*/
;***** 162      switch ( AH(pRegister) ) {
	MOV	ECX,[EBP+08H];	pRegister
	XOR	EAX,EAX
	MOV	AL,[ECX+01dH]
	JMP	BLBL13
	ALIGN 04H
BLBL14:

;***** 163      case FUNC_OPENPDD:                 // open the PDD
;***** 164           PRINTDEBUG(szFunc1);
	PUSH	OFFSET FLAT:szFunc1
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 165           if ( fOpened ) {              // if already open
	CMP	DWORD PTR  fOpened,0H
	JE	FELB3

;***** 166                fRetval = FALSE;         // its an error
	MOV	DWORD PTR [EBP-04H],0H;	fRetval
FELB3:

;***** 167           }
;***** 168           fRetval = VDHOpen(szDigioPDD, &hDigio, &ulActionTaken,
	PUSH	OFFSET FLAT:szDigioPDD
	PUSH	OFFSET FLAT:hDigio
	PUSH	OFFSET FLAT:ulActionTaken
	PUSH	0H
	PUSH	0H
	PUSH	01H
	PUSH	02040H
	PUSH	0H
	CALL	VDHOPEN
	MOV	[EBP-04H],EAX;	fRetval

;***** 169                        0, VDHOPEN_FILE_NORMAL, VDHOPEN_FILE_OPEN,
;***** 170                        VDHOPEN_FLAGS_FAIL_ON_ERROR | VDHOPEN_SHARE_DENYNONE, 0);
;***** 171 
;***** 172           if ( fRetval ) {              // if everything is ok
	CMP	DWORD PTR [EBP-04H],0H;	fRetval
	JE	FELB4

;***** 173                fOpened = TRUE;          // we are open
	MOV	DWORD PTR  fOpened,01H
FELB4:

;***** 174           }
;***** 175           break;
	JMP	BLBL12
	ALIGN 04H
BLBL15:

;***** 176 
;***** 177      case FUNC_CLOSEPDD:                // Schlieáen des PDD durchfhren
;***** 178           PRINTDEBUG(szFunc2);
	PUSH	OFFSET FLAT:szFunc2
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 179           fRetval = FALSE;              // Per Default Fehler eintragen
	MOV	DWORD PTR [EBP-04H],0H;	fRetval

;***** 180           if ( ! fOpened ) {            // if we didn't open
	CMP	DWORD PTR  fOpened,0H
	JNE	FELB5

;***** 181                break;                   // indicate error
	JMP	BLBL12
	ALIGN 04H
FELB5:

;***** 182           }
;***** 183           VDHClose(hDigio);             // close the device
	MOV	AX,WORD PTR  hDigio
	PUSH	EAX
	CALL	VDHCLOSE

;***** 184           fOpened = FALSE;              // No longer open
	MOV	DWORD PTR  fOpened,0H

;***** 185           fRetval = TRUE;               // its ok now
	MOV	DWORD PTR [EBP-04H],01H;	fRetval

;***** 186           break;
	JMP	BLBL12
	ALIGN 04H
BLBL16:

;***** 187 
;***** 188 
;***** 189           /*------------------------------------------------*/
;***** 190           /* DANGER !!!!! DANGER !!!!!! DANGER !!!!!! DANGER*/
;***** 191           /*                                                */
;***** 192           /* If you use a PDD and IOCtls the PDD normally   */
;***** 193           /* does a LockSeg on the buffers passed. This will*/
;***** 194           /* fail if the buffer comes from a VDD, because   */
;***** 195           /* the VMM does a lock itsself, so that the buffer*/
;***** 196           /* comes to the PDD locked.                       */
;***** 197           /* Therefore the PDD has to check the typeProcess */
;***** 198           /* field in the LocalInfoSeg for 1 (DOS-Process)  */
;***** 199           /* If this is 1, there has to be done no locking  */
;***** 200           /* and of course no unlocking.                    */
;***** 201           /* If you block, you have to save this information*/
;***** 202           /* for instance in the RPstatus-field of your     */
;***** 203           /* Request-packet. Because this local-info-seg    */
;***** 204           /* will be unavailable later on.                  */
;***** 205           /*                                                */
;***** 206           /* Verify access is fine (no problem)             */
;***** 207           /*                                                */
;***** 208           /* DANGER !!!!! DANGER !!!!!! DANGER !!!!!! DANGER*/
;***** 209           /*------------------------------------------------*/
;***** 210 
;***** 211      case FUNC_SUBFUNC1:                // Subfunction 1 (Write byte to port)
;***** 212           PRINTDEBUG(szFunc3);
	PUSH	OFFSET FLAT:szFunc3
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 213           if ( ! fOpened ) {            // if we are not open
	CMP	DWORD PTR  fOpened,0H
	JNE	FELB6

;***** 214                fRetval = FALSE;         // indicate error
	MOV	DWORD PTR [EBP-04H],0H;	fRetval

;***** 215                break;                   // and go out
	JMP	BLBL12
	ALIGN 04H
FELB6:

;***** 216           }
;***** 217           fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 1,
	MOV	AX,WORD PTR  hDigio
	PUSH	EAX
	PUSH	091H
	PUSH	01H
	PUSH	DWORD PTR  pParameter
	PUSH	DWORD PTR  ulLenMaxParam
	PUSH	OFFSET FLAT:ulBytesPassedParam
	PUSH	DWORD PTR  pBuffer
	PUSH	DWORD PTR  ulLenMaxBuffer
	PUSH	OFFSET FLAT:ulBytesPassedBuffer
	CALL	VDHDEVIOCTL
	MOV	[EBP-04H],EAX;	fRetval

;***** 218                          pParameter, ulLenMaxParam, &ulBytesPassedParam,
;***** 219                          pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
;***** 220           break;
	JMP	BLBL12
	ALIGN 04H
BLBL17:

;***** 221 
;***** 222      case FUNC_SUBFUNC2:                // Subfunction 2 (Read byte from port)
;***** 223           PRINTDEBUG(szFunc4);
	PUSH	OFFSET FLAT:szFunc4
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 224           if ( ! fOpened ) {            // if we are not open
	CMP	DWORD PTR  fOpened,0H
	JNE	FELB7

;***** 225                fRetval = FALSE;         // indicate error
	MOV	DWORD PTR [EBP-04H],0H;	fRetval

;***** 226                break;                   // and go out
	JMP	BLBL12
	ALIGN 04H
FELB7:

;***** 227           }
;***** 228           fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 2,
	MOV	AX,WORD PTR  hDigio
	PUSH	EAX
	PUSH	091H
	PUSH	02H
	PUSH	DWORD PTR  pParameter
	PUSH	DWORD PTR  ulLenMaxParam
	PUSH	OFFSET FLAT:ulBytesPassedParam
	PUSH	DWORD PTR  pBuffer
	PUSH	DWORD PTR  ulLenMaxBuffer
	PUSH	OFFSET FLAT:ulBytesPassedBuffer
	CALL	VDHDEVIOCTL
	MOV	[EBP-04H],EAX;	fRetval

;***** 229                          pParameter, ulLenMaxParam, &ulBytesPassedParam,
;***** 230                          pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
;***** 231           break;
	JMP	BLBL12
	ALIGN 04H
BLBL18:

;***** 232 
;***** 233      case FUNC_SUBFUNC3:                // Subfunction 3 (read immediate)
;***** 234           PRINTDEBUG(szFunc5);
	PUSH	OFFSET FLAT:szFunc5
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 235           if ( ! fOpened ) {            // if we are not open
	CMP	DWORD PTR  fOpened,0H
	JNE	FELB8

;***** 236                fRetval = FALSE;         // indicate error
	MOV	DWORD PTR [EBP-04H],0H;	fRetval

;***** 237                break;                   // and go out
	JMP	BLBL12
	ALIGN 04H
FELB8:

;***** 238           }
;***** 239           fRetval = VDHDevIOCtl(hDigio, DIGIO_CATEGORY, 3,
	MOV	AX,WORD PTR  hDigio
	PUSH	EAX
	PUSH	091H
	PUSH	03H
	PUSH	DWORD PTR  pParameter
	PUSH	DWORD PTR  ulLenMaxParam
	PUSH	OFFSET FLAT:ulBytesPassedParam
	PUSH	DWORD PTR  pBuffer
	PUSH	DWORD PTR  ulLenMaxBuffer
	PUSH	OFFSET FLAT:ulBytesPassedBuffer
	CALL	VDHDEVIOCTL
	MOV	[EBP-04H],EAX;	fRetval

;***** 240                          pParameter, ulLenMaxParam, &ulBytesPassedParam,
;***** 241                          pBuffer, ulLenMaxBuffer, &ulBytesPassedBuffer);
;***** 242           break;
	JMP	BLBL12
	ALIGN 04H
	JMP	BLBL12
	ALIGN 04H
BLBL13:
	CMP	EAX,01H
	JE	BLBL14
	CMP	EAX,02H
	JE	BLBL15
	CMP	EAX,03H
	JE	BLBL16
	CMP	EAX,04H
	JE	BLBL17
	CMP	EAX,05H
	JE	BLBL18
BLBL12:

;***** 243      }
;***** 244 
;***** 245 
;***** 246      if ( fRetval ) {                   // if everything is ok
	CMP	DWORD PTR [EBP-04H],0H;	fRetval
	JE	FELB9

;***** 247           AX(pRegister) = 0;            // set the ok-Flag (0) in AX
	MOV	EAX,[EBP+08H];	pRegister
	MOV	WORD PTR [EAX+01cH],0H

;***** 248           PRINTDEBUG(szReqTRUE);
	PUSH	OFFSET FLAT:szReqTRUE
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H
	JMP	FELB10
	ALIGN 04H
FELB9:

;***** 249      } else {
;***** 250           /*------------------------------------------------*/
;***** 251           /* Scan for the last error                        */
;***** 252           /*------------------------------------------------*/
;***** 253           pRegister->crf_eax = VDHGetError();
	MOV	ESI,[EBP+08H];	pRegister
	CALL	VDHGETERROR
	MOV	ECX,EAX
	MOV	EAX,ESI
	MOV	[EAX+01cH],ECX

;***** 254           PRINTDEBUG(szReqFALSE);
	PUSH	OFFSET FLAT:szReqFALSE
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H
FELB10:

;***** 255      }
;***** 256 
;***** 257      VDHPopInt();                       // remove IRET-frame on the stack
	CALL	VDHPOPINT

;***** 258      return(TRUE) ;                     // don't chain to the next VDD
	MOV	EAX,01H
	JMP	FELB11
	ALIGN 04H

;***** 259 }
FELB11:
	ADD	ESP,08H
	POP	ESI
	LEAVE	
	RET	04H
DEMOINTHANDLER	ENDP

;***** 279 PVOID CalcLinearAdress(USHORT usSeg, USHORT usOff)
	ALIGN 04H

	PUBLIC CalcLinearAdress
CalcLinearAdress	PROC
	PUSH	EBP
	MOV	EBP,ESP
	SUB	ESP,04H
	MOV	[EBP+08H],AX;	usSeg
	MOV	[EBP+0cH],DX;	usOff

;***** 283      ulAdress = (ULONG) usSeg;
	XOR	EAX,EAX
	MOV	AX,[EBP+08H];	usSeg
	MOV	[EBP-04H],EAX;	ulAdress

;***** 284      ulAdress <<= 4;
	MOV	EAX,[EBP-04H];	ulAdress
	SAL	EAX,04H
	MOV	[EBP-04H],EAX;	ulAdress

;***** 285      ulAdress += (ULONG) usOff;
	XOR	EAX,EAX
	MOV	AX,[EBP+0cH];	usOff
	ADD	EAX,[EBP-04H];	ulAdress
	MOV	[EBP-04H],EAX;	ulAdress

;***** 286      ulAdress += (ULONG) CurrVDM;
	MOV	EAX,DWORD PTR  CurrVDM
	ADD	EAX,[EBP-04H];	ulAdress
	MOV	[EBP-04H],EAX;	ulAdress

;***** 287      return((PVOID) ulAdress);
	MOV	EAX,[EBP-04H];	ulAdress
	JMP	FELB19
	ALIGN 04H
FELB19:
	LEAVE	
	RET	
CalcLinearAdress	ENDP

;***** 300 BOOL HOOKENTRY DemoDosTerminated(HVDM hVdm)
	ALIGN 04H

	PUBLIC DEMODOSTERMINATED
DEMODOSTERMINATED	PROC
	PUSH	EBP
	MOV	EBP,ESP

;***** 303      if ( fOpened ) {                   // if the channel is yet open
	CMP	DWORD PTR  fOpened,0H
	JE	FELB20

;***** 304           VDHClose(hDigio);             // close it -> PDD likes it
	MOV	AX,WORD PTR  hDigio
	PUSH	EAX
	CALL	VDHCLOSE
FELB20:

;***** 305      }
;***** 306 
;***** 307      PRINTDEBUG(szMessage4);            // control-message
	PUSH	OFFSET FLAT:szMessage4
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 308      return(TRUE);                      // always the same
	MOV	EAX,01H
	JMP	FELB21
	ALIGN 04H

;***** 309 }
FELB21:
	LEAVE	
	RET	04H
DEMODOSTERMINATED	ENDP

;***** 329 LONG EXPENTRY OS2ReqFunc(SGID SGid, ULONG ulCmd, ULONG cbIn, PVOID pReqIn,
	ALIGN 04H

	PUBLIC OS2REQFUNC
OS2REQFUNC	PROC
	PUSH	EBP
	MOV	EBP,ESP

;***** 334      PRINTDEBUG(szMessage5);            // control-message
	PUSH	OFFSET FLAT:szMessage5
	MOV	AL,01H
	CALL	_printf
	ADD	ESP,04H

;***** 335 
;***** 336      return(VDDREQ_PASS);               // pass on to next VDD in chain
	OR	EAX,0ffffffffH
	JMP	FELB22
	ALIGN 04H
FELB22:
	LEAVE	
	RET	018H
OS2REQFUNC	ENDP

;***** 357 BOOL EXPENTRY VDDInit(PSZ pszConfig)
	ALIGN 04H
CODE32	ENDS
CINIT_TEXT	SEGMENT

	PUBLIC VDDINIT
VDDINIT	PROC
	PUSH	EBP
	MOV	EBP,ESP

;***** 359      PRINTDEBUG(szInitMessage);         // Display message
	PUSH	OFFSET FLAT:szInitMessage
	MOV	AL,01H
	CALL	_printf

;***** 360 
;***** 361      /*------------------------------------------------*/
;***** 362      /* Install both hooks for the VDM                 */
;***** 363      /*------------------------------------------------*/
;***** 364      if ( ! VDHInstallUserHook(VDM_CREATE, DemoDosCreated) ) {
	PUSH	0H
	PUSH	OFFSET FLAT: DEMODOSCREATED
	CALL	VDHINSTALLUSERHOOK
	ADD	ESP,04H
	OR	EAX,EAX
	JNE	FELB23

;***** 365           return(0);                    // produce error
	XOR	EAX,EAX
	JMP	FELB24
	ALIGN 04H
FELB23:

;***** 366      }
;***** 367 
;***** 368      if ( ! VDHInstallUserHook(VDM_TERMINATE, DemoDosTerminated) ) {
	PUSH	01H
	PUSH	OFFSET FLAT: DEMODOSTERMINATED
	CALL	VDHINSTALLUSERHOOK
	OR	EAX,EAX
	JNE	FELB25

;***** 369           return(0);                    // produce error
	XOR	EAX,EAX
	JMP	FELB24
	ALIGN 04H
FELB25:

;***** 370      }
;***** 371 
;***** 372      /*------------------------------------------------*/
;***** 373      /* install a hook for DosRequestVDD               */
;***** 374      /* !!! Brian !!!                                  */
;***** 375      /*------------------------------------------------*/
;***** 376      if ( ! VDHRegisterVDD(szDriverName, OS2ReqFunc, NULL) ) {
	PUSH	OFFSET FLAT:szDriverName
	PUSH	OFFSET FLAT: OS2REQFUNC
	PUSH	0H
	CALL	VDHREGISTERVDD
	OR	EAX,EAX
	JNE	FELB26

;***** 377           return(0);                    // produce an error
	XOR	EAX,EAX
	JMP	FELB24
	ALIGN 04H
FELB26:

;***** 378      }
;***** 379 
;***** 380      return(1);                         // we are happy
	MOV	EAX,01H
	JMP	FELB24
	ALIGN 04H

;***** 381 }
FELB24:
	LEAVE	
	RET	04H
VDDINIT	ENDP
CINIT_TEXT	ENDS
END	VDDINIT
