/* $Header: sdf.c,v 1.6 93/06/14 10:16:47 mikeh Exp $ */

/*
 * SDF <file>
 *
 *         Print the headers in the SDF <file> specified
 *
 * Copyright 1989-1992 Hewlett-Packard Company
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <SDF_comm.h>
#include <SDF_util.h>


/*  Tables for enumerated integers
 */
typedef struct
{
    short enumVal;
    char *enumStr;
} ENUM_STRUCT_STR;

ENUM_STRUCT_STR enum_application[] = {
    { -1,	"HP VISTA" },
    { -2,	"HP SINE" },
    { -3,	"HP35660" },
    { -4,	"HP3562/63" },
    { -5,	"HP3588" },
    { -6,	"HP3589" },
    { 1,	"HP3566/67" },
    { 2,	"HP35665" },
    { 3,	"HP3560" },
    { 4,	"HP89410/440" },
    { 5,	"calibration" },
    { 7,	"HP35635R" },
    { 8,	"HP35654A-S1A" },
    { 9,	"HP3569" },
    { 10,	"HP35670" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_averageType[] = {
    { AVERAGE_OFF,        "off" },
    { RMS_AVERAGE,        "rms" },
    { RMS_EXPO,	          "rms exponential" },
    { VECTOR_AVERAGE,     "vector" },
    { VECTOR_EXPO,        "vector exponential" },
    { CONT_PEAK_HOLD,     "continuous peak hold" },
    { PEAK_AVERAGE,       "peak" },
    { TIME_AVERAGE,       "time" },
    { CONT_TIME_AVERAGE,  "continuous time" },
    { TIME_EXPO_AVERAGE,  "time exponential" },
    { EQUAL_CONF_AVERAGE, "equal conf" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_measType[] = {
    { UNKNOWN_MEAS,       "unknown" },
    { SPECTRUM_MEAS,      "spectrum" },
    { NETWORK_MEAS,       "network" },
    { SWEPT_MEAS,	  "swept" },
    { FFT_MEAS,	          "fft" },
    { ORDERS_MEAS,        "orders" },
    { OCTAVE_MEAS,        "octave" },
    { CAPTURE_MEAS,       "capture" },
    { CORRELATION_MEAS,   "correlation" },
    { HISTOGRAM_MEAS,     "histogram" },
    { SWEPT_NETWORK_MEAS, "swept network" },
    { FFT_NETWORK_MEAS,   "fft network" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_realTime[] = {
    { 0,  "not continuous" },
    { 1,  "continuous" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_detection[] = {
    { UNKNOWN_DETECT,   "unknown" },
    { SAMPLE_DETECT,    "sample" },
    { POS_PEAK_DETECT,  "positive peak" },
    { NEG_PEAK_DETECT,  "negative peak" },
    { ROSENFELL_DETECT, "rosenfell" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_domain[] = {
    { UNKNOWN_DOMAIN,   "unknown" },
    { FREQ_DOMAIN,      "frequency" },
    { TIME_DOMAIN,      "time" },
    { AMPLITUDE_DOMAIN, "amplitude" },
    { RPM_DOMAIN,	"RPM" },
    { ORDER_DOMAIN,     "order" },
    { CHANNEL_DOMAIN,   "channel" },
    { OCTAVE_DOMAIN,    "octave" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_dataType[] = {
    { UNKNOWN_DATA,	        "unknown" },
    { TIME_DATA,		"time" },
    { LINEAR_SPC_DATA, 	        "linear spectrum" },
    { AUTO_PWR_DATA,	        "auto-power spectrum" },
    { CROSS_PWR_DATA,	        "cross-power spectrum" },
    { FREQ_RESP_DATA,	        "frequency response" },
    { AUTO_CORREL_DATA,	        "auto-correlation" },
    { CROSS_CORREL_DATA,	"cross-correlation" },
    { IMPL_RESP_DATA,           "impulse response" },
    { ORD_COH_DATA,             "ordinary coherence" },
    { PART_COH_DATA,            "partial coherence" },
    { MULT_COH_DATA,	        "multiple coherence" },
    { FULL_OCTAVE_DATA,	        "full octave" },
    { THIRD_OCTAVE_DATA,	"third octave" },
    { CONVOLUTION_DATA,	        "convolution" },
    { HISTOGRAM_DATA,           "histogram" },
    { PRO_DENSITY_FUNC_DATA,    "probability density function" },
    { CUM_DENSITY_FUNC_DATA,    "cumulative density function" },
    { PWR_SPC_ORDER_TRACK_DATA, "power spectrum order tracking" },
    { COMP_PWR_TRACK_DATA,      "composite order tracking" },
    { PHASE_ORDER_TRACK_DATA,   "phase order tracking" },
    { RPM_SPECTRAL_DATA,	"rpm spectral" },
    { ORDER_RATIO_DATA,	        "order ratio" },
    { ORBIT_DATA,		"orbit" },
    { SINE_RMS_PWR_DATA,	"sine rms power" },
    { SINE_VARIANCE_DATA,	"sine variance" },
    { SINE_RANGE_DATA, 	        "sine range" },
    { SINE_SETTLE_TIME_DATA,    "sine settle time" },
    { SINE_INTEG_TIME_DATA,     "sine integrate time" },
    { SINE_SOURCE_DATA,	        "sine source" },
    { SINE_OVERLOAD_DATA,	"sine overload" },
    { SINE_LINEAR_DATA,	        "sine linear" },
    { SYNTHESIS,		"synthesis" },
    { WEIGHT_DATA,	        "weight" },
    { FREQ_CORRECTION_DATA,     "frequency correction" },
    { ALL_PASS_TIME_DATA,	"all pass time" },
    { NORM_REFERENCE_DATA,      "normal reference" },
    { TACH_DATA,		"tach" },
    { LIMIT_LINE_DATA,	        "limit line" },
    { TWELFTH_OCTAVE_DATA,      "twelfth octave" },
    { S11_DATA,		        "s11" },
    { S21_DATA,		        "s21" },
    { S12_DATA,		        "s12" },
    { S22_DATA,		        "s22" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_xResolution[] = {
    { LINEAR_RES,    "linear" },
    { LOG_RES,       "logarithmic" },
    { ARB_XPMeasHdr, "arbitrary, one per file" },
    { ARB_XPDataHdr, "arbitrary, one per data type" },
    { ARB_XPVectHdr, "arbitrary, one per trace" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_data_type[] = {
    { SHORT16,  "short" },
    { LONG32,   "long" },
    { FLOAT32,  "float" },
    { DOUBLE64, "double" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_weight[] = {
    { NO_WEIGHTING, "no weighting" },
    { A_WEIGHTING,  "A weighting" },
    { B_WEIGHTING,  "B weighting" },
    { C_WEIGHTING,  "C weighting" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_windowType[] = {
    { WINDOW_NOT_APPLIED, "not applied" },
    { HANNING, 	          "hanning" },
    { FLAT_TOP,	          "flat top" },
    { UNIFORM, 	          "uniform" },
    { FORCE,		  "force" },
    { EXPONENTIAL,	  "exponential (response)" },
    { USER_WINDOW,	  "user" },
    { HAMMING, 	          "hamming" },
    { P301,		  "p301" },
    { P310,		  "p310" },
    { KAISER_BESSEL,	  "kaiser bessel" },
    { HARRIS,		  "harris" },
    { BLACKMAN,	          "blackman" },
    { RESOLUTION_FILTER,  "resolution filter" },
    { CORREL_LEAD_LAG,    "correlation lead lag" },
    { CORREL_LAG,         "correlation lag" },
    { GATED,              "gated" },
    { P400,               "p400" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_windowCorrMode[] = {
    { CORR_NOT_APPLIED, "not applied" },
    { NARROW,           "narrow" },
    { WIDE,	        "wide" },
    { USER_CORR,	"user" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_direction[] = {
    { -TZ_DIRECTION, "-TZ" },
    { -TY_DIRECTION, "-TY" },
    { -TX_DIRECTION, "-TX" },
    { -Z_DIRECTION,  "-Z" },
    { -Y_DIRECTION,  "-Y" },
    { -X_DIRECTION,  "-X" },
    { NO_DIRECTION,  "none" },
    { X_DIRECTION,   "X" },
    { Y_DIRECTION,   "Y" },
    { Z_DIRECTION,   "Z" },
    { R_DIRECTION,   "R" },
    { T_DIRECTION,   "T" },
    { P_DIRECTION,   "P" },
    { TX_DIRECTION,  "TX" },
    { TY_DIRECTION,  "TY" },
    { TZ_DIRECTION,  "TZ" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_coupling[] = {
    { DC_COUPLING, "DC" },
    { AC_COUPLING, "AC" },
    { 0,  NULL }
};

ENUM_STRUCT_STR enum_channelAttribute[] = {
    { UNKNOWN_ATTRIB,	         "unknown" },
    { NO_ATTRIB,		 "none" },
    { TACH_ATTRIB,	         "tach" },
    { REFERENCE_ATTRIB,	         "reference" },
    { TACH_AND_REFERENCE_ATTRIB, "tach & reference" },
    { 0,  NULL }
};



/*  Generate error message and exit program
 */
void error (char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    vfprintf (stderr, fmt, args);
    fprintf (stderr, "\n");
    va_end (args);
    exit (-1);
}


/*  Look up enum in passed table, if not found, then return "# is UNKNOWN"
 */
char *strEnum (short sval, ENUM_STRUCT_STR *enumStruct)
{
    static char buf[20];
    for ( ; enumStruct->enumStr; enumStruct++)
        if (enumStruct->enumVal == sval)
	    return enumStruct->enumStr;
    sprintf (buf, "%d is UNKNOWN", sval);
    return buf;
}


/*  Print the line to stdout, right justifying the title so that the
 *  printed values line up.
 */
void print (char *title, char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    printf ("%18s: ", title);
    vprintf (fmt, args);
    printf ("\n");
    va_end (args);
}


/*  Print a title and string to stdout
 */
void pStr (char *title, char *s)
{
    print (title, "%s", s);
}


/*  Print a title and enumerated integer to stdout
 */
void pEnum (char *title, short sval, ENUM_STRUCT_STR *enumStruct)
{
    pStr  (title, strEnum (sval, enumStruct) );
}


/*  Print a title and boolean to stdout
 */
void pBool (char *title, short sval)
{
    pStr  (title, (sval ? "yes" : "no") );
}


/*  Print a title and a 16 bit integer (short) to stdout
 */
void pShrt (char *title, short sval)
{
    print (title, "%d", sval);
}


/*  Print a title and a 32 bit integer (long) to stdout
 */
void pLong (char *title, long lval)
{
    print (title, "%ld", lval);
}


/*  Print a title and a 32 bit real (float) to stdout
 */
void pFlot (char *title, float fval)
{
    print (title, "%g", fval);
}


/*  Print a title and a 64 bit real (double) to stdout
 */
void pDoub (char *title, double dval)
{
    print (title, "%lg", dval);
}


/*  Print a row of dots as a separator
 */
void pDots (void)
{
    int cnt = 18;
    while (--cnt >= 0)
	putchar ('.');
    putchar ('\n');
}


/*  Print the passed file header to stdout
 */
void printFileHeader (Sdf_FILE_HDR *filePtr)
{
    char *suffix = "am";
    int month = filePtr->monthDayStamp / 100;
    int day = filePtr->monthDayStamp % 100;
    int hour = filePtr->hourMinStamp / 100;
    int minute = filePtr->hourMinStamp % 100;

    if (hour >= 12) 
	suffix = "pm";
    if (hour > 12) 
	hour -= 12;

    pDots ();
    pStr  ("FILE HEADER",    "");
    pShrt ("record type",    filePtr->recordType);
    pLong ("record size",    filePtr->recordSize);
    pShrt ("revision",       filePtr->revisionNum);
    pEnum ("application",    filePtr->applic, enum_application);
    print ("date stamp",     "%d-%d-%d", month, day, filePtr->yearStamp);
    print ("time stamp",     "%d:%02d %s", hour, minute, suffix);
    pStr  ("app version",    filePtr->applicVer);
    pShrt ("data count",     filePtr->num_of_DATA_HDR_record);
    pLong ("data offset",    filePtr->offset_of_DATA_HDR_record);
    pShrt ("vector count",   filePtr->num_of_VECTOR_record);
    pLong ("vector offset",  filePtr->offset_of_VECTOR_record);
    pShrt ("channel count",  filePtr->num_of_CHANNEL_record);
    pLong ("channel offset", filePtr->offset_of_CHANNEL_record);
    pShrt ("unique count",   filePtr->num_of_UNIQUE_record);
    pLong ("unique offset",  filePtr->offset_of_UNIQUE_record);
    pShrt ("scanstr count",  filePtr->num_of_SCAN_STRUCT_record);
    pLong ("scanstr offset", filePtr->offset_of_SCAN_STRUCT_record);
    pShrt ("xdata count",    filePtr->num_of_XDATA_record);
    pLong ("xdata offset",   filePtr->offset_of_XDATA_record);
    pLong ("ydata offset",   filePtr->offset_of_YDATA_record);

#if SDF_REV >= 3
    pShrt ("scanbig count",  filePtr->num_of_SCAN_BIG_record);
    pShrt ("comment count",  filePtr->num_of_COMMENT_record);
    pLong ("scanbig offset", filePtr->offset_of_SCAN_BIG_record);
    pLong ("comment offset", filePtr->offset_of_COMMENT_record);
    pLong ("next SDF offset",filePtr->offset_of_next_SDF_FILE);
#endif /* SDF_REV >= 3 */

    printf("\n");
}


/*  Print the passed measurement header to stdout
 */
void printMeasHeader (Sdf_MEAS_HDR *measPtr)
{
    pDots ();
    pStr  ("MEASUREMENT HEADER","");
    pShrt ("record type",      measPtr->recordType);
    pLong ("record size",      measPtr->recordSize);
    pLong ("unique rec",       measPtr->unique_record);
    pFlot ("center freq old",  measPtr->centerFreqOld);
    pFlot ("span freq old",    measPtr->spanFreqOld);
    pLong ("block size",       measPtr->blockSize);
    pBool ("zoom mode",        measPtr->zoomModeOn);

#if SDF_REV >= 3
    pShrt ("start freq inx old", measPtr->startFreqIndexOld);
    pShrt ("stop freq inx old",  measPtr->stopFreqIndexOld);
#else
    pShrt ("start freq index", measPtr->startFreqIndex);
    pShrt ("stop freq index",  measPtr->stopFreqIndex);
#endif /* SDF_REV >= 3 */

    pEnum ("average type",     measPtr->averageType, enum_averageType);
    pLong ("average number",   measPtr->averageNum);
    print ("percent overlap",  "%g%%", measPtr->pctOverlap * 100);
    pStr  ("measurement title",measPtr->measTitle);
    pFlot ("video bandwidth",  measPtr->videoBandWidth);
    pDoub ("center freq",      measPtr->centerFreq);
    pDoub ("span freq",        measPtr->spanFreq);
    pDoub ("sweep freq",       measPtr->sweepFreq);
    pEnum ("meas type",        measPtr->measType, enum_measType);
    pEnum ("real time",        measPtr->realTime, enum_realTime);
    pEnum ("detection",        measPtr->detection, enum_detection);
    pDoub ("sweep time",       measPtr->sweepTime);

#if SDF_REV >= 3
    pLong ("start freq index", measPtr->startFreqIndex);
    pLong ("stop freq index",  measPtr->stopFreqIndex);
#endif /* SDF_REV >= 3 */

    printf("\n");
}


/*  Print the passed data header to stdout
 *  The passed result is only there to be printed.
 */
void printDataHeader (Sdf_DATA_HDR *dataPtr, int result)
{
    pDots ();
    print ("DATA HEADER",       "record %d", result);
    pShrt ("record type",       dataPtr->recordType);
    pLong ("record size",       dataPtr->recordSize);
    pLong ("unique rec",        dataPtr->unique_record);
    pStr  ("data title",        dataPtr->dataTitle);
    pEnum ("domain",            dataPtr->domain, enum_domain);
    pEnum ("data type",         dataPtr->dataType, enum_dataType);

#if SDF_REV >= 3
    pShrt ("num of points old", dataPtr->num_of_pointsOld);
    pShrt ("last valid inx old",dataPtr->last_valid_indexOld);
#else
    pShrt ("number of points",  dataPtr->num_of_points);
    pShrt ("last valid index",  dataPtr->last_valid_index);
#endif /* SDF_REV >= 3 */

    pFlot ("absc first x old",  dataPtr->abscissa_firstXOld);
    pFlot ("absc delta x old",  dataPtr->abscissa_deltaXOld);
    pEnum ("x resolution type", dataPtr->xResolution_type, enum_xResolution);
    pEnum ("x data type",       dataPtr->xdata_type, enum_data_type);
    pShrt ("x per point",       dataPtr->xPerPoint);
    pEnum ("y data type",       dataPtr->ydata_type, enum_data_type);
    pShrt ("y per point",       dataPtr->yPerPoint);
    pBool ("y is complex",      dataPtr->yIsComplex);
    pBool ("y is normalized",   dataPtr->yIsNormalized);
    pBool ("y is power data",   dataPtr->yIsPowerData);
    pBool ("y is valid",        dataPtr->yIsValid);
    pLong ("first vec rec #",   dataPtr->first_VECTOR_recordNum);
    pShrt ("total rows",        dataPtr->total_rows);
    pShrt ("total cols",        dataPtr->total_cols);
    pStr  ("x unit label",      dataPtr->xUnit.label);
    pFlot ("x unit factor",     dataPtr->xUnit.factor);
    pShrt ("x unit mass",       (short) dataPtr->xUnit.mass);
    pShrt ("x unit length",     (short) dataPtr->xUnit.length);
    pShrt ("x unit time",       (short) dataPtr->xUnit.time);
    pShrt ("x unit current",    (short) dataPtr->xUnit.current);
    pShrt ("x unit temp",       (short) dataPtr->xUnit.temperature);
    pShrt ("x unit lum intens", (short) dataPtr->xUnit.luminal_intensity);
    pShrt ("x unit mole",       (short) dataPtr->xUnit.mole);
    pShrt ("x unit plane angle",(short) dataPtr->xUnit.plane_angle);
    pStr  ("y unit label",      dataPtr->yUnit.label);
    pFlot ("y unit factor",     dataPtr->yUnit.factor);
    pShrt ("y unit mass",       (short) dataPtr->yUnit.mass);
    pShrt ("y unit length",     (short) dataPtr->yUnit.length);
    pShrt ("y unit time",       (short) dataPtr->yUnit.time);
    pShrt ("y unit current",    (short) dataPtr->yUnit.current);
    pShrt ("y unit temp",       (short) dataPtr->yUnit.temperature);
    pShrt ("y unit lum intens", (short) dataPtr->yUnit.luminal_intensity);
    pShrt ("y unit mole",       (short) dataPtr->yUnit.mole);
    pShrt ("y unit plane angle",(short) dataPtr->yUnit.plane_angle);
    pDoub ("abscissa first x",  dataPtr->abscissa_firstX);
    pDoub ("abscissa delta x",  dataPtr->abscissa_deltaX);
    pBool ("scan data",         dataPtr->scanData);
    pBool ("window applied",    dataPtr->windowApplied);

#if SDF_REV >= 3
    pLong ("number of points",  dataPtr->num_of_points);
    pLong ("last valid index",  dataPtr->last_valid_index);
    pShrt ("oversample factor", dataPtr->overSampleFactor);
    pShrt ("multipass mode",    dataPtr->multiPassMode);
    pShrt ("multipass decimat", dataPtr->multiPassDecimations);
#endif /* SDF_REV >= 3 */

    printf("\n");
}


/*  Print the passed vector header to stdout
 *  The passed result, row and col are only there to be printed.
 */
void printVectorHeader (Sdf_VECTOR_HDR *vectPtr, 
			int result, int row, int col)
{
    pDots ();
    print ("VECTOR HEADER",    "data %d, row %d, column %d", 
			       result, row, col);
    pShrt ("record type",      vectPtr->recordType);
    pLong ("record size",      vectPtr->recordSize);
    pLong ("unique rec",       vectPtr->unique_record);
    print ("channel record",   "(%d,%d)", vectPtr->the_CHANNEL_record[0], 
					  vectPtr->the_CHANNEL_record[1] );
    print ("power of channel", "(%d,%d)", vectPtr->pwrOfChan[0], 
					  vectPtr->pwrOfChan[1] );
    printf("\n");
}


/*  Print the passed vector header to stdout
 *  The passed channel is only there to be printed.
 */
void printChannelHeader (Sdf_CHANNEL_HDR *chanPtr, int chan)
{
    pDots ();
    print ("CHANNEL HEADER",    "record %d", chan);
    pShrt ("record type",       chanPtr->recordType);
    pLong ("record size",       chanPtr->recordSize);
    pLong ("unique rec",        chanPtr->unique_record);
    pStr  ("label",             chanPtr->channelLabel);
    pStr  ("module ID",         chanPtr->moduleId);
    pStr  ("serial number",     chanPtr->serialNum);
    pEnum ("window type",       chanPtr->window.windowType, enum_windowType);
    pEnum ("correction mode",   chanPtr->window.windowCorrMode, 
			        enum_windowCorrMode);
    pFlot ("bandwidth",         chanPtr->window.windowBandWidth);
    pFlot ("time constant",     chanPtr->window.windowTimeConst);
    pFlot ("truncation",        chanPtr->window.windowTrunc);
    pFlot ("wide band corr",    chanPtr->window.wideBandCorr);
    pFlot ("narrow band corr",  chanPtr->window.narrowBandCorr);
    pEnum ("weighting",         chanPtr->weight, enum_weight);

#if SDF_REV >= 3
    pFlot ("delay old",         chanPtr->delayOld);
#else
    pFlot ("delay",             chanPtr->delay);
#endif /* SDF_REV >= 3 */

    pFlot ("range",             chanPtr->range);
    pEnum ("direction",         chanPtr->direction, enum_direction);
    pShrt ("point number",      chanPtr->pointNum);
    pEnum ("coupling",          chanPtr->coupling, enum_coupling);
    pBool ("overloaded",        chanPtr->overloaded);
    pStr  ("internal label",    chanPtr->intLabel);
    pStr  ("eng unit label",    chanPtr->engUnit.label);
    pFlot ("eng unit factor",   chanPtr->engUnit.factor);
    pShrt ("eng unit mass",     (short) chanPtr->engUnit.mass);
    pShrt ("eng unit length",   (short) chanPtr->engUnit.length);
    pShrt ("eng unit time",     (short) chanPtr->engUnit.time);
    pShrt ("eng unit current",  (short) chanPtr->engUnit.current);
    pShrt ("eng unit temp",     (short) chanPtr->engUnit.temperature);
    pShrt ("eng unit lum inten",(short) chanPtr->engUnit.luminal_intensity);
    pShrt ("eng unit mole",     (short) chanPtr->engUnit.mole);
    pShrt ("eng unit plane ang",(short) chanPtr->engUnit.plane_angle);
    pFlot ("inter to EU factor",chanPtr->int2engrUnit);
    pFlot ("input impedance",   chanPtr->inputImpedance);
    pEnum ("attribute",         chanPtr->channelAttribute, 
				enum_channelAttribute);
    pBool ("alias protected",   chanPtr->aliasProtected);
    pBool ("digital",           chanPtr->digital);
    pDoub ("scale",             chanPtr->channelScale);
    pDoub ("offset",            chanPtr->channelOffset);
    pDoub ("gate begin",        chanPtr->gateBegin);
    pDoub ("gate end",          chanPtr->gateEnd);
    pDoub ("user delay",        chanPtr->userDelay);

#if SDF_REV >= 3
    pDoub ("delay",             chanPtr->delay);
    pDoub ("carrier freq",      chanPtr->carrierFreq);
    pShrt ("channel #",         chanPtr->channelNumber);
    pShrt ("channel module",    chanPtr->channelModule);
#endif /* SDF_REV >= 3 */

    printf("\n");
}


/*  This is it
 */
int main (int argc, char * argv[] )
{
    short result, row, col, chan;
    short fileNum;
    Sdf_FILE_HDR fileHdr;
    Sdf_MEAS_HDR measHdr;
    Sdf_DATA_HDR dataHdr;
    Sdf_VECTOR_HDR vectHdr;
    Sdf_CHANNEL_HDR chanHdr;

    if (argc != 2)
	error ("usage: %s <filename>", argv[0] );

    if ( (fileNum = SdfOpen (argv[1]) ) == 0)
	error ("SdfOpen of '%s' failed", argv[1] );

    /*  read File Header
     */
    if (SdfFileHdr (fileNum, &fileHdr) )
	error ("SdfFileHdr failed");
    printFileHeader (&fileHdr);

    /*  read Measurement Header
     */
    if (SdfMeasHdr (fileNum, &measHdr) )
	error ("SdfMeasHdr failed");
    printMeasHeader (&measHdr);

    /*  read all the Data Headers
     */
    for (result = 0; result < fileHdr.num_of_DATA_HDR_record; result++)
    {
	if (SdfDataHdr (fileNum, result, &dataHdr) )
	    error ("SdfDataHdr failed");
        printDataHeader (&dataHdr, result);

        /* read the Vector Headers associated with this data header 
         */
	for (col = 0; col < dataHdr.total_cols; col++)
	{
	    for (row = 0; row < dataHdr.total_rows; row++)
	    {
		if (SdfVectHdr (fileNum, result, row, col, &vectHdr) )
		    error ("SdfVectHdr failed");
		printVectorHeader (&vectHdr, result, row, col);
	    }
	}
    }

    /*  Read all the Channel Headers
     */
    for (chan = 0; chan < fileHdr.num_of_CHANNEL_record; chan++)
    {
	if (SdfChanHdr (fileNum, chan, &chanHdr) )
	    error ("SdfChanHdr failed");
	printChannelHeader (&chanHdr, chan);
    }

    if (SdfClose (fileNum) )
	error ("SdfClose failed");

    return 0;
}
