/*----------------------------------------------------------------------------
     Modul     :    DEMODD.H

     Meaning   :    VDD-Header for INT-Handler (Demo)

     Autor     :    Michael Glienecke

     History   :    23.02.1993
*----------------------------------------------------------------------------*/
BOOL EXPENTRY  VDDInit(PSZ);            // INIT-Func
LONG EXPENTRY  OS2ReqFunc(SGID, ULONG, ULONG, PVOID, ULONG, PVOID);
BOOL HOOKENTRY DemoDosCreated(HVDM);    // Creation-Hook-Function
BOOL HOOKENTRY DemoDosTerminated(HVDM); // Termination-Hook-Function
BOOL HOOKENTRY DemoIntHandler(PCRF);    // INT-Handler
PVOID          CalcLinearAdress(USHORT, USHORT);

int  _System _printf(char *, ...);      // Redefinition that it's going
#define   printf    _printf             // otherwise the linker is complaining

#define   FALSE     0                   // I like the manifest constants
#define   TRUE      1
