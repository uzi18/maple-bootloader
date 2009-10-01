#ifndef __MAPLE_DFU_H
#define __MAPLE_DFU_H

#include "maple_lib.h"

/* externed globals */
extern u32 userAppAddr; /* points to the start of users vector table (user app 0x00) */

/* exposed types */
typedef u8*(*ClassReqCB)(u16);

/* exposed structs */
typedef struct _DFUStatus {
  u8 bStatus;
  u8 bwPollTimeout0;
  u8 bwPollTimeout1;
  u8 bwPollTimeout2;
  u8 bState;  /* state of device at the time the host receives the message! */
  u8 iString;
} DFUStatus;


/* ideally we could wrap usb requests (that are dfu specific) into a dfu
   event struct, but given that the global ptrs pInfo/property are forced
   on us by usb_lib, we might as well use them. Todo: dont use them ;) */
/* typedef struct _DFUEvent { */
/* } DFUEvent; */


/* exposed enums */
typedef enum _DFUbRequest {
                  /* bmRequestType, wValue,    wIndex,    wLength, Data */
  DFU_DETACH,     /* 0x21,          wTimeout,  Interface, Zero,    None */
  DFU_DNLOAD,     /* 0x21,          wBlockNum, Interface, Length,  Firmware */
  DFU_UPLOAD,     /* 0xA1,          Zero,      Interface, Length,  Firmware */
  DFU_GETSTATUS,  /* 0xA1,          Zero,      Interface, 6,       Status */
  DFU_CLRSTATUS,  /* 0x21,          Zero,      Interface, Zero,    None */
  DFU_GETSTATE,   /* 0xA1,          Zero,      Interface, 1,       State */
  DFU_ABORT,      /* 0x21,          Zero,      Interface, Zero,    None */
} DFUbRequest;

/* to avoid a bunch of extra casting (to u8), dont enumerate status/states */

/*** DFU Status Values ******/
#define  OK              0x00 /* No error */
#define  errTARGET       0x01 /* File is not appropriate for this device */
#define  errFILE         0x02 /* File fails some vendor tests */
#define  errWRITE        0x03 /* Device is unable to write memory */
#define  errERASE        0x04 /* Memory erase failed */
#define  errCHECK_ERASED 0x05 /* Memory erase check failed */
#define  errPROG         0x06 /* Program memory function failed */
#define  errVERIFY       0x07 /* Written program failed verification */
#define  errADDRESS      0x08 /* address out of range */
#define  errNOTDONE      0x09 /* received DNLOAD with wLength=0, but firmware seems incomplete */
#define  errFIRMWARE     0x0A /* Runtime firmware corrupt, cannot return to non-dfu operations! */
#define  errVENDOR       0x0B /* vendor specific error */
#define  errUSBR         0x0C /* Unexpected usb reset! */
#define  errPOR          0x0D /* Unexpected power on reset */
#define  errUNKNOWN      0x0E /* Unknown error */
#define  errSTALLEDPKT   0x0F /* device stalled unexpected request */
/***************************/

/*** DFU State Values **************/
#define  appIDLE                0x00
#define  appDETACH              0x01
#define  dfuIDLE                0x02
#define  dfuDNLOAD_SYNC         0x03
#define  dfuDNBUSY              0x04
#define  dfuDNLOAD_IDLE         0x05
#define  dfuMANIFEST_SYNC       0x06
#define  dfuMANIFEST            0x07
#define  dfuMANIFEST_WAIT_RESET 0x08
#define  dfuUPLOAD_IDLE         0x09
#define  dfuERROR               0x0A
/***********************************/





/* exposed functions */
void dfuInit(void);  /* singleton dfu initializer */

/* should consume dfuEvent type, but for now we can use pInfo (see comment above) */
void dfuUpdateState(void); 

/* hooks in with how usb_core expects class resquests to be handled by copy routines */
ClassReqCB dfuGetClassHandler(void); 

/* cruft */
/* DFUState dfuGetState(); */ /* negated by use of global dfuAppStatus */
/* void dfuSetState(DFUState); */ /* just unecessary */



#endif
