/*
 * Metrolab device support
 */

#include <epicsStdio.h>
#include <devCommonGpib.h>

/******************************************************************************
 *
 * The following define statements are used to declare the names to be used
 * for the dset tables.   
 *
 * A DSET_AI entry must be declared here and referenced in an application
 * database description file even if the device provides no AI records.
 *
 ******************************************************************************/
#define DSET_AI     devAiMetrolab   // analog in   - into EPICS
#define DSET_AO     devAoMetrolab   // analog out  - out to device
#define DSET_BI     devBiMetrolab   // binary in
#define DSET_BO     devBoMetrolab   // binary out
#define DSET_SI     devSiMetrolab   // string in
#define DSET_SO     devSoMetrolab   // string out
#define DSET_LI     devLiMetrolab   // long in
#define DSET_LO     devLoMetrolab   // long out
//#define DSET_EV     devEvMetrolab
//#define DSET_MBBI   devMbbiMetrolab
//#define DSET_MBBID  devMbbidMetrolab
//#define DSET_MBBO   devMbboMetrolab
//#define DSET_MBBOD  devMbbodMetrolab
//#define DSET_WF     devWfMetrolab

#include <devGpib.h> /* must be included after DSET defines */

#define TIMEOUT     1.0    /* I/O must complete within this time */
#define TIMEWINDOW  2.0    /* Wait this long after device timeout */

/******************************************************************************
 * Strings used by the init routines to fill in the znam,onam,...
 * fields in BI and BO record types.
 ******************************************************************************/

static char  *offOnList[] = { "Off","On" };
static struct devGpibNames   offOn = { 2,offOnList,0,1 };

static char  *remoteList[] = { "Remote","Remote" };
static struct devGpibNames remote = { 2,remoteList,0,1 };

static char  *localList[] = { "Local","Local" };
static struct devGpibNames local = { 2,localList,0,1 };

//static char  *initNamesList[] = { "Init","Init" };
//static struct devGpibNames initNames = { 2,initNamesList,0,1 };
//
//static char  *disableEnableList[] = { "Disable","Enable" };
//static struct devGpibNames disableEnable = { 2,disableEnableList,0,1 };
//
//static char  *resetList[] = { "Reset","Reset" };
//static struct devGpibNames reset = { 2,resetList,0,1 };
//
//static char  *lozHizList[] = { "50 OHM","IB_Q_HIGH Z" };
//static struct devGpibNames lozHiz = {2,lozHizList,0,1};
//
//static char  *invertNormList[] = { "INVERT","NORM" };
//static struct devGpibNames invertNorm = { 2,invertNormList,0,1 };
//
//static char  *fallingRisingList[] = { "FALLING","RISING" };
//static struct devGpibNames fallingRising = { 2,fallingRisingList,0,1 };
//
//static char  *clearList[] = { "Clear","Clear" };
//static struct devGpibNames clear = { 2,clearList,0,1 };

/******************************************************************************
 * Structures used by the init routines to fill in the onst,twst,... and the
 * onvl,twvl,... fields in MBBI and MBBO record types.
 *
 * Note that the intExtSsBm and intExtSsBmStop structures use the same
 * intExtSsBmStopList and intExtSsBmStopVal lists but have a different number
 * of elements in them that they use... The intExtSsBm structure only represents
 * 4 elements,while the intExtSsBmStop structure represents 5.
 ******************************************************************************/
/*
  static char *intExtSsBmStopList[] = {
  "INTERNAL","EXTERNAL","SINGLE SHOT","BURST MODE","STOP" };
  static unsigned long intExtSsBmStopVal[] = { 0,1,2,3,2 };
  static struct devGpibNames intExtSsBm = {
  4,intExtSsBmStopList, intExtSsBmStopVal,2 };
  static struct devGpibNames intExtSsBmStop = {
  5,intExtSsBmStopList,intExtSsBmStopVal,3 };
*/

/******************************************************************************
 * String arrays for EFAST operations. The last entry must be 0.
 *
 * On input operations,only as many bytes as are found in the string array
 * elements are compared.  Additional bytes are ignored.
 * The first matching string  will be used as a match.
 *
 * For the input operations,the strings are compared literally!  This
 * can cause problems if the instrument is returning things like \r and \n
 * characters.  When defining input strings so you include them as well.
 ******************************************************************************/

//static char *userOffOn[] = {"USER OFF;","USER ON;",0};

/******************************************************************************
 * Array of structures that define all GPIB messages
 * supported for this type of instrument.
 ******************************************************************************/

static struct gpibCmd gpibCmds[] = {
  /* Param 0 -- Read the displayed value */
  {&DSET_SI,      // DSET of command (read a string from the device)
   GPIBREAD,      // Type (read)
   IB_Q_HIGH,     // Priority (high)
   "\x05",        // Format string to send to device (ASCII HEX 05 - <ENQ>)
   "%s",          // String to interpret message
   0, 200,        // Error message length, return message length
   NULL,          // Conversion
   0, 0, NULL,    // P1, P2, and P3 used in conversion
   NULL,          // Name strings
   NULL           // Input end-of-string
  },

  /* Param 1 - Remote control */
  {&DSET_BO, GPIBCMD, IB_Q_HIGH, "R", NULL, 0, 0, NULL, 0, 0, NULL, &remote, NULL},

  /* Param 2 - Local control */
  {&DSET_BO, GPIBCMD, IB_Q_HIGH, "L", NULL, 0, 0, NULL, 0, 0, NULL, &local, NULL},

  /* Param 3 -- Read and convert the displayed value */
  {&DSET_SI,      // DSET of command (read a string from the device)
   GPIBREAD,      // Type (read)
   IB_Q_HIGH,     // Priority (high)
   "\x05",        // Format string to send to device (ASCII HEX 05 - <ENQ>)
   "%*c%9s",          // String to interpret message (NULL because we'll convert it)
   0, 200,        // Error message length, return message length
   NULL,          // Conversion
   0, 0, NULL,    // P1, P2, and P3 used in conversion
   NULL,          // Name strings
   NULL           // Input end-of-string
  },


//  {&DSET_BO, GPIBCMD, IB_Q_HIGH, "*RST", NULL, 0, 80, NULL, 0, 0, NULL, &reset, NULL},
//
//  /* Param 2 - SCPI clear status command */
//  {&DSET_BO, GPIBCMD, IB_Q_HIGH, "*CLS", NULL, 0, 80, NULL, 0, 0, NULL, &clear, NULL},
//
//  /* Param 3 - Read SCPI status byte */
//  {&DSET_LI, GPIBREAD, IB_Q_HIGH, "*STB?", "%d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 4 - Read SCPI event register */
//  {&DSET_LI, GPIBREAD, IB_Q_HIGH, "*ESR?", "%d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 5 - Enable SCPI events */
//  {&DSET_LO, GPIBWRITE, IB_Q_HIGH, NULL, "*ESE %d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 6 - Read back SCPI enabled events */
//  {&DSET_LI, GPIBREAD, IB_Q_HIGH, "*ESE?", "%d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 7 - Enable SCPI service request sources */
//  {&DSET_LO, GPIBWRITE, IB_Q_HIGH, NULL, "*SRE %d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 8 - Read back SCPI enabled service request sources */
//  {&DSET_LI, GPIBREAD, IB_Q_HIGH, "*SRE?", "%d", 0, 80, NULL, 0, 0, NULL, NULL, NULL},
//
//  /* Param 9 - Read SCPI output completion status */
//  {&DSET_LI, GPIBREAD, IB_Q_HIGH, "*OPC?", "%d", 0, 80, NULL, 0, 0, NULL, NULL, 0},
//
//
//  /* Some more examples */
//  {&DSET_BO, GPIBCMD, IB_Q_HIGH, "init", 0, 0, 32, NULL, 0, 0, NULL, &initNames, 0},
//  {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, 0, NULL, 0, 32, 0, 0, 0, userOffOn, &offOn, 0},
//  {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "user?", 0, 0, 32, 0, 0, 0, userOffOn, &offOn, 0},
//  {&DSET_AI, GPIBREAD, IB_Q_LOW, "send", "%lf", 0, 32, 0, 0, 0, NULL, NULL, 0}
};

/* The following is the number of elements in the command array above.  */
#define NUMPARAMS sizeof(gpibCmds)/sizeof(struct gpibCmd)



/*
 * Conversion Routines
 */




/******************************************************************************
 * Initialize device support parameters
 *
 *****************************************************************************/
static long init_ai(int parm)
{
  if(parm==0) {
    devSupParms.name = "devMetrolab";
    devSupParms.gpibCmds = gpibCmds;
    devSupParms.numparams = NUMPARAMS;
    devSupParms.timeout = TIMEOUT;
    devSupParms.timeWindow = TIMEWINDOW;
    devSupParms.respond2Writes = -1;
  }
  return(0);
}
