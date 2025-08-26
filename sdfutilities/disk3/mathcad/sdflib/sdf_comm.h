#ifndef SDF_comm_h
#define SDF_comm_h
/****************************************************************************
 * $Copyright: (C) 1988-1993, Hewlett-Packard Co. All rights reserved.$
 *
 * $Header: SDF_comm.h,v 1.15 93/03/16 17:03:11 mikeh Exp $
 *
 * $Module name: SDF_comm.h $
 * Product:      SDF shared w/ instruments, PCs, & workstations (s300, s700)
 *
 * $Revision: 1.15 $
 * $Date: 93/03/16 17:03:11 $
 * RCS $State: Exp $
 *
 * $Contributor: Lake Stevens Instrument Division $
 *
 * $Description: SDF common data structures include file $
 *
 ****************************************************************************/

#include <SDF_basi.h>



/*************************************************************************
 *  NOTE:                                                                *
 *                                                                       *
 * . SDF ASSUMES DATA IS SAVED IN INTERNAL UNIT (NO ENGR UNIT APPLIED!)  *
 * . IN MIMO CASE, SDF ASSUMES ROW IS THE REFERENCE CHANNELS AND         *
 *   COL IS THE RESPONSES CHANNELS. WHEN SAVE IN SDF FILE, ROW IS        *
 *   LOOPPED MORE FREQUENTLY THAN COL.                                   * 
 *************************************************************************/


/*-----------------------------------------------------------------------*
 |    General "SDF" File                                                 |
 |                                                                       |
 | "1" formatType char[FORMAT_TYPE_CHAR_SIZE] character string           |
 |     (null-terminated)                                                 |
 | "1" SDF_FILE_HDR record                                               |
 | "1" SDF_MEAS_HDR record                                               |
 | "num_of_DATA_HDR_record" SDF_DATA_HDR record                          |
 | "num_of_VECTOR_record" SDF_VECTOR_HDR record                          |
 | "num_of_CHANNEL_record" SDF_CHANNEL_HDR record                        |
 | "num_of_UNIQUE_record" UNIQUE record                                  |
 | "num_of_SCAN_STRUCT_record" SDF_SCAN_STRUCT record with "num_of_scan" |
 |          values in the record                                         |
 | "num_of_SCAN_BIG_record" SDF_SCAN_BIG and/or                          |
 |          SDF_SCAN_VAR records with "num_of_scan" scan values in       |
 |          SDF_SCAN_VAR records                                         |
 | "num_of_COMMENT_record" SDF_COMMENT_HDR record                        |
 | "0 or 1" SDF_HDR (recordType=SDF_XDATA_TYPE) with arbitrary size      |
 |          CONTIGUOUS xData                                             |
 | "1" SDF_HDR (recordType=SDF_YDATA_TYPE) with arbitrary size           |
 |     CONTIGUOUS yData                                                  |
 |                                                                       |
 |-----------------------------------------------------------------------|
 |                                                                       |
 |    "SDF" Non-waterfall One Vector Trace Measurement Data              |
 |                                                                       |
 | "1  " formatType char[FORMAT_TYPE_CHAR_SIZE] character string         |
 |       (null-terminated)                                               |
 | "1  " SDF_FILE_HDR record                                             |
 | "1  " SDF_MEAS_HDR record                                             |
 | "1  " SDF_DATA_HDR record                                             |
 | "0 or 1" SDF_VECTOR_HDR record                                        |
 | "num_of_CHANNEL_record" SDF_CHANNEL_HDR record                        |
 | "num_of_UNIQUE_record" UNIQUE record                                  |
 | "0 or 1" SDF_HDR (recordType=SDF_XDATA_TYPE) with arbitrary size      |
 |          CONTIGUOUS xData                                             |
 | "1  " SDF_HDR (recordType=SDF_YDATA_TYPE) with arbitrary size         |
 |       CONTIGUOUS yData                                                |
 |                                                                       |
 |-----------------------------------------------------------------------|
 |                                                                       |
 |   Minimum Records Required in "SDF" File                              |
 |                                                                       |
 | "1  " formatType char[FORMAT_TYPE_CHAR_SIZE] character string         |
 |       (null-terminated)                                               |
 | "1  " SDF_FILE_HDR record                                             |
 | "1  " SDF_MEAS_HDR record                                             |
 | "1  " SDF_DATA_HDR record                                             |
 | "1  " SDF_HDR (recordType=SDF_YDATA_TYPE) with arbitrary size         |
 |       CONTIGUOUS yData                                                |
 |                                                                       |
 *-----------------------------------------------------------------------*/



/******************************************************************************
 * Label         factor    M  L  T  C  K  I ML RD 
 * -----------------------------------------------
 * lbm           0.45359   1
 * in            2.54e-2   0  1
 * ft	         0.3048	   0  1
 * 
 * s             1.0       0  0  1
 * h             3600.0	   0  0  1
 * min           60.0      0  0  1
 * 
 * lb            4.4482	   1  1  -2
 * lbf           4.4482	   1  1  -2
 * in/s          2.54e-2   0  1  -1
 * in/s^2        2.54e-2   0  1  -2
 * g's           9.80665   0  1  -2
 * psi           6894.8    1  -1 -2
 * 
 * BTU           1055.1    1  2  -2
 * W/m^2         1.0       1  0  -3	      
 * BTU/h*ft^2    3.1547    1  0  -3 	  
 * 
 * hz            6.28319   0  0  -1 0  0  0  0  1
 * rpm           0.10472   0  0  -1 0  0  0  0  1
 * 
 * A             1.0       0  0  0  1
 * W             1.0       1  2  -3 
 * V             1.0       1  2  -3 -1
 * Ohm           1.0       1  2  -3 -2
 * pC            1.0e-12   0  0  1  1
 * -----------------------------------------------
 *
 * mass, length, time, current, temperature,luminal_intensity,
 * mole and plane_angle are eight different dimensions to
 * define different units.
 *  
 * In SDF, all eight dimensions are multipled by PWR_OF_UNIT_MULTIPLIER.
 * If you are interested in further coding idea, look at function:
 * set_unit() in SDF_def.c file.
 * 
 * In SDF, units are coded as:   
 *    hz
 *    label:		hz
 *    factor:		6.28319
 *    mass: 		0
 *    length:		0
 *    time:		-2
 *    current:		0
 *    temperature:	0
 *    luminal_intensity:0
 *    mole:             0
 *    plane_angle:      2
 *
 *    V
 *    label:		V
 *    factor:		1.0
 *    mass:		2
 *    length:		4
 *    time:		-6
 *    current:		-2
 *    temperature:	0
 *    luminal_intensity:0
 *    mole:		0
 *    plane_angle:	0 
 ******************************************************************************
 */
typedef struct SDF_UNIT {
    char label[UNIT_LABEL_CHAR_SIZE];
    FLOATSIZ32 factor;
    char mass;
    char length;
    char time;
    char current;
    char temperature;
    char luminal_intensity;
    char mole;
    char plane_angle;
} Sdf_UNIT, PTR32 *Sdf_UNIT_PTR;


/******************************************************************************
 * windowType:      type of window (Window_type enum)
 * windowCorrMode:  0=CORR_NO_APPLIED
 *                  1=NARROW
 *                  2=WIDE
 *                  3=USER_CORR
 * windowBandWith:  window band width
 * windowTimeConst: decay for FORCE and EXPONENTIAL windowType
 * windowTrunc:     width for FORCE and EXPONENTIAL windowType
 * narrowBandCorr:  narrow band correction factor
 * wideBandCorr:    wide band correction factor
 ******************************************************************************
 */
typedef struct SDF_WINDOW {
    SHORTSIZ16 windowType;      /* Window_type enum*/
    SHORTSIZ16 windowCorrMode;  /* Window_corr_mode enum*/
    FLOATSIZ32 windowBandWidth;
    FLOATSIZ32 windowTimeConst;
    FLOATSIZ32 windowTrunc;
    FLOATSIZ32 wideBandCorr;
    FLOATSIZ32 narrowBandCorr;
} Sdf_WINDOW, PTR32 *Sdf_WINDOW_PTR;


/******************************************************************************
 * SDF_HDR is the record header for recordType = SDF_XDATA_TYPE or
 * SDF_YDATA_TYPE.
 * For SDF_XDATA_TYPE:
 *     recordType = SDF_XDATA_TYPE
 *     recordSize = sizeof(struct SDF_HDR) + total byte size of all
 *                  x data values in the entire file
 *     x data values followed right after recordType and recordSize
 * For SDF_YDATA_TYPE:
 *     recordType = SDF_YDATA_TYPE
 *     recordSize = sizeof(struct SDF_HDR) + total byte size of all
 *                  y data values in the entire file
 *     y data values followed right after recordType and recordSize
 ******************************************************************************
 */
typedef struct SDF_HDR {
    SHORTSIZ16 recordType;
    LONGSIZ32  recordSize;
} Sdf_HDR, PTR32 *Sdf_HDR_PTR;


/******************************************************************************
 * FIRST FIELD IN SDF FILE:
 *
 * SDF in BINARY format:
 *     ascii_or_bin_char = 'B'
 *     last_char         = '\0'
 * SDF in ASCII format: NOT SUPPORTED
 *     ascii_of_bin_char = 'A'
 *     last_char         = '\0'
 ******************************************************************************
 */
typedef struct FORMAT_STRUCT {
    char ascii_or_bin_char;
    char last_char;
} Format_STRUCT, PTR32 *Format_STRUCT_PTR;


/******************************************************************************
 * recordType=     SDF_FILE_HDR_TYPE
 * recordSize=     sizeof(struct SDF_FILE_HDR) in bytes
 * revisionNum=    SDF_REV; current SDF revision structure
 * applic:         application which generates this SDF file
 * yearStamp:      YEAR when this measurement was started. (0-xxxxx)
 * monthDayStamp:  MONTH and DAY when this measurement was started (0-1231)
 *                 Encoded as (MONTH * 100) + DAY.
 *                 For instance, Dec 31 = 1231
 *                               Feb 19 = 219
 *                               Jan 1  = 101
 * hourMinStamp:   HOUR and MINUTE taken when this measurement was started.
 *                 (0-2359) Encoded as (HOUR * 100) + MINUTE.
 *                 For instance, 23:59  = 2359
 *                                0:00  = 0 
 *                                1:10  = 110
 * applicVer:      application revision number (null terminated)
 * num_of_DATA_HDR_record:    total SDF_DATA_HDR records in this file (1 to n)
 * num_of_VECTOR_record:      total SDF_VECTOR records in this file.  (0 to n)
 * num_of_CHANNEL_record:     total SDF_CHANNEL records in this file. (0 to n)
 * num_of_UNIQUE_record:      total UNIQUE records in this file.      (0 to n)
 * num_of_SCAN_STRUCT_record: total SDF_SCAN_STRUCT records in file.  (0 to 1)
 * num_of_XDATA_record:       total XDATA record in this file.        (0 to 1)
 *
 * NOTE: The following offset fields are referenced from the beginning of
 *       the logical file. The field contains INVALID_OFFSET if no
 *       records of that type are in the file.
 *
 * offset_of_DATA_HDR_record:    offset of the first SDF_DATA_HDR record.
 * offset_of_VECTOR_record:      offset of the first SDF_VECTOR_HDR record.
 * offset_of_CHANNEL_record:     offset of the first SDF_CHANNEL_HDR record.
 * offset_of_UNIQUE_record:      offset of the first UNIQUE record.
 * offset_of_SCAN_STRUCT_record: offset of the SDF_SCAN_STRUCT record.
 * offset_of_XDATA_record:       offset of SDF_HDR (SDF_XDATA_TYPE),
 *                               INVALID_OFFSET if no x data values.
 * offset_of_YDATA_record:   offset of SDF_HDR(SDF_YDATA_TYPE).
 *
 *                           SDF-3
 * num_of_BIG_SCAN_record:   number of SDF_SCAN_BIG & SDF_SCAN_VAR 
 *                           records.  (0 to n)
 * num_of_COMMENT_record:    number of SDF_COMMENT_HDR records.  (0 to n)
 * offset_of_BIG_SCAN_record: offset of the SDF_SCAN_BIG & SDF_SCAN_VAR 
 *                            records.
 * offset_of_COMMENT_record: offset of first SDF_COMMENT_HDR record.
 * offset_of_next_SDF_FILE:  supports putting more than one logical SDF 
 *    FILE in a file to support multiple results taken at the same time
 *    that are completely independant. For example, time capture
 *    where the span/center freq of each channel of data are
 *    completely unrelated. This offset points to the FORMAT_STRUCT
 *    record of the next logical SDF FILE in this physical file.  All
 *    offsets in the next SDF FILE are relative to the start of the
 *    FORMAT_STRUCT of the logical file.
 ******************************************************************************
 */
typedef struct SDF_FILE_HDR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    SHORTSIZ16  revisionNum;
    SHORTSIZ16  applic;         /* Applic enum */
    SHORTSIZ16  yearStamp;
    SHORTSIZ16  monthDayStamp;
    SHORTSIZ16  hourMinStamp;
    char        applicVer[APPLIC_VER_CHAR_SIZE];
    SHORTSIZ16  num_of_DATA_HDR_record;
    SHORTSIZ16  num_of_VECTOR_record;
    SHORTSIZ16  num_of_CHANNEL_record;
    SHORTSIZ16  num_of_UNIQUE_record;
    SHORTSIZ16  num_of_SCAN_STRUCT_record;
    SHORTSIZ16  num_of_XDATA_record;
    LONGSIZ32   offset_of_DATA_HDR_record;
    LONGSIZ32   offset_of_VECTOR_record;
    LONGSIZ32   offset_of_CHANNEL_record;
    LONGSIZ32   offset_of_UNIQUE_record;
    LONGSIZ32   offset_of_SCAN_STRUCT_record;
    LONGSIZ32   offset_of_XDATA_record;
    LONGSIZ32   offset_of_YDATA_record;

#if SDF_REV >= 3
    SHORTSIZ16  num_of_SCAN_BIG_record;
    SHORTSIZ16  num_of_COMMENT_record;
    LONGSIZ32   offset_of_SCAN_BIG_record;
    LONGSIZ32   offset_of_COMMENT_record;
    LONGSIZ32   offset_of_next_SDF_FILE;
#endif /* SDF_REV >= 3 */

} Sdf_FILE_HDR, PTR32 *Sdf_FILE_HDR_PTR;


/******************************************************************************
 * This structure is an alternate way of describing the beginning of the
 * SDF file which is the Sdf_FORMAT structure followed by the Sdf_FILE_HDR
 * structure (used by SDF_func.c and SDF_file.c).
 ******************************************************************************
 */
typedef struct sdf_file {
    char          format[FORMAT_TYPE_CHAR_SIZE];
    Sdf_FILE_HDR  fileHdr;
} Sdf_file, PTR32 *Sdf_file_PTR;


/******************************************************************************
 * recordType=     SDF_MEAS_TYPE
 * recordSize=     sizeof(struct SDF_MEAS_TYPE) in bytes
 * unique_record:  If SDF is used as the internal data structure, this
 *                 field contains the internal memory address of this record's 
 *                 unique record.  Ignore this field during save/recall.
 * centerFreqOld:  Old instrument center frequency (use centerFreq)
 * spanFreqOld:    Old instrument span frequency (use spanFreq)
 * blockSize:      Block size
 * zoomModeOn:     0 = zoomMode is off, 1 = zoomMode is on.
 * startFreqIndexOld: alias start index (only used for frequency domain data)
 * stopFreqIndexOld:  alias stop index  (only used for frequency domain data)
 * averageType:    average type (Average_type enum)
 * averageNum:     number of averages
 * pctOverlap:     0-100%, percentage of overlap used for averaging measurement
 * measTitle:      measurement title or auto math label (null terminated)
 * videoBandWidth: video band width
 *
 *              SDF-2
 * centerFreq:  instrument center frequency
 * spanFreq:    instrument span frequency
 * sweepFreq:   Current frequency for swept measurement
 * measType:    measurement type (Measure_type enum)
 * realTime:    0 = block or swept, 1 = continuous
 * detection:   type of data detection (Detect_type enum)
 * sweepTime:   time for a swept measurement
 *
 *              SDF-3
 * startFreqIndex: alias start index (only used for frequency domain data)
 * stopFreqIndex:  alias stop index  (only used for frequency domain data)
 * expAverageNum:  number of exponential averages
 ******************************************************************************
 */
typedef struct SDF_MEAS_HDR {
    SHORTSIZ16     recordType;
    LONGSIZ32      recordSize;
    UNIQUE_PTR     unique_record;

#if SDF_REV >= 2
    FLOATSIZ32     centerFreqOld;    /* Old, SUPPORT LIMITED */
    FLOATSIZ32     spanFreqOld;      /* Old, SUPPORT LIMITED */
#else
    FLOATSIZ32     centerFreq;
    FLOATSIZ32     spanFreq;
#endif /* SDF_REV >= 2 */

    LONGSIZ32      blockSize;
    SHORTSIZ16     zoomModeOn;
#if SDF_REV >= 3
    SHORTSIZ16     startFreqIndexOld;
    SHORTSIZ16     stopFreqIndexOld;
#else
    SHORTSIZ16     startFreqIndex;
    SHORTSIZ16     stopFreqIndex;
#endif /* SDF_REV >= 3 */
    SHORTSIZ16     averageType;      /* Average_type enum */
    LONGSIZ32      averageNum;
    FLOATSIZ32     pctOverlap;
    char           measTitle[MEAS_TITLE_CHAR_SIZE];
    FLOATSIZ32     videoBandWidth;

#if SDF_REV >= 2
    FLOATSIZ64     centerFreq; 
    FLOATSIZ64     spanFreq;
    FLOATSIZ64     sweepFreq;
    SHORTSIZ16     measType;         /* Measure_type enum */
    SHORTSIZ16     realTime;         /* 1=continuous */
    SHORTSIZ16     detection;        /* Detect_type enum */
    FLOATSIZ64     sweepTime;
#endif /* SDF_REV >= 2 */

#if SDF_REV >= 3
    LONGSIZ32      startFreqIndex;
    LONGSIZ32      stopFreqIndex;
    FLOATSIZ64     expAverageNum;
#endif /* SDF_REV >= 3 */

} Sdf_MEAS_HDR, PTR32 *Sdf_MEAS_HDR_PTR;


/******************************************************************************
 * recordType=    SDF_DATA_TYPE;
 * recordSize=    sizeof(struct SDF_DATA_HDR) in bytes
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record. Ignore this field during save/recall
 * dataTitle:     Title of data (null-terminated), defines in SDF_basi.h
 * domain:    domain of data (e.g. time, freq; see Domain enum)
 * dataType:  type of data (e.g. spectrum, time; see Data_type enum)
 * num_of_pointsOld:    number of data points for this measurement type data
 * last_valid_indexOld: 0 to (num_of_points-1)
 * abscissa_firstXOld: Old field, may not be supported in future
 * abscissa_deltaXOld: Old field, may not be supported in future
 * xResolution_type:   0=LINEAR_RES; 
 *                         No x data vector stored in XDATA record for this 
 *                         SDF_DATA_HDR record. x data is evenly linear spaced.
 *                             Xn=abscissa_firstX + n * abscissa_deltaX
 *                     1=LOG_RES;
 *                         No x data vector stored in XDATA record for this 
 *                         SDF_DATA_HDR. x data is evenly log spaced.
 *                             Xn=abscissa_firstX * n * abscissa_deltaX
 *                     2=ARB_XPMeasHdr; 
 *                         Only one x data vector stored in SDF_XDATA_HDR,
 *                         common to all SDF_VECTOR_HDR in all SDF_DATA_HDR.
 *                     3=ARB_XPDataHdr; 
 *                         All vectors within the same SDF_DATA_HDR record 
 *                         share the same x data value vector in XDATA record.
 *                     4=ARB_XPVectHdr; 
 *                         Each vector within the same SDF_DATA_HDR has a 
 *                         unique x data value vector.
 * xdata_type:   1=SHORT16, 2=LONG32, 3=FLOAT32, 4=DOUBLE64 (Number_type)
 * xPerPoint:    number of x values per each data point
 * ydata_type:   1=SHORT16, 2=LONG32, 3=FLOAT32, 4=DOUBLE64 (Number_type)
 * yPerPoint:    number of y values per each data point
 *               NOTE: 1 (real+imaginary) considered as 1 yPerPoint
 *                     1 (real)           considered as 1 yPerPoint
 *                     2 (real+imaginary) considered as 2 yPerPoint
 *                     2 (real)           considered as 2 yPerPoint
 *                     1 (mag+phase)      considered as 2 yPerPoint
 *                     multi-pass data    considered as 2 yPerPoint
 * yIsComplex:         0=real, 1=real+imaginary
 * yIsNormalized:      0=not normalized;
 *                     1=normalized; y data between 0.0 to 1.0
 * yIsPowerData:       0=non-power data (e.g. Linear Spectrum, Histogram)
 *                     1=power data (e.g. Auto Power Spectrum)
 * yIsValid:           0=data is invalid, 1=data is valid
 * first_VECTOR_recordNum: (0 to num_of_VECTOR_record-1)
 *                         first SDF_VECTOR_HDR record belongs to this
 *                         SDF_DATA_HDR record.  For instance, if there are 
 *                         2 SDF_DATA_HDR records and each has 3 
 *                         SDF_VECTOR_HDRs. Then 1st's 
 *                         first_VECTOR_recordNum=0, and 2nd's=3. 
 * total_rows:  Total rows in this dataType. (min. value is "1") 
 *              For single channel results, this is related to the channel #.
 * total_cols:  Total cols in this dataType. (min. value is "1")
 *              This field is not currently used by any known instrument.
 *
 * Total number of SDF_VECTOR_HDR records belonging to this
 * SDF_DATA_HDR is total_rows * total_cols
 *
 * xUnit:       Unit for x axis
 * yUnitValid:  0=yUnit is invalid, 1=yUnit is valid
 *              If the engineering unit for y axis is derived from 
 *              SDF_CHANNEL_HDR and SDF_VECTOR_HDR, then this flag must 
 *              be set to INVALID.
 * yUnit:       Unit for y axis when yUnitValid is 1.
 *              This unit is used ONLY when the y data unit is not
 *              derived from engUnit in SDF_CHAN_HDR.
 *
 *                  SDF-2
 * abscissa_firstX: If xResolution_type is LINEAR_RES or LOG_RES,
 *                  this field contains the first x value, use
 *                  abscissa_firstX, abscissa_deltaX, num_of_points,
 *                  xdata_type and xPerPoint to figure out x data values.
 *                  Otherwise, figure out where this SDF_DATA_HDR's
 *                  x data values start in SDF_HDR(SDF_XDATA_TYPE)
 *                  and depends on num_of_points, xResolution_type,
 *                  xdata_type and xPerPoint to read x data values from SDF_HDR
 * abscissa_deltaX: If xResolution_type is LINEAR_RES or LOG_RES,
 *                  this field indicates the delta between each two X
 *                  values.  For multi-pass data, this value applies to the 
 *                  highest frequency span.  If the res is not LINEAR_RES or
 *                  LOG_RES, then ignore this field.
 * scanData:        0 = This DATA_HDR is associated with non-scanned YDATA.
 *                  1 = This DATA_HDR is associated with the SCAN_STRUCT.
 *                  This field is only used if there is a valid 
 *                  SCAN_STRUCT for this file.  Both scanned and
 *                  non-scanned data may be present in 1 file.
 *                  ALL SCANNED DATA RESULTS MUST BE CONTIGUOUS
 *                  RESULTS for scan_type=SCAN
 * windowApplied:   Whether the windows indicated have already been
 *                  applied to the data.
 *
 *                   SDF-3
 * num_of_points:    number of data points for this measurement type data
 * last_valid_index: 0 to (num_of_points-1)
 * overSampleFactor: Usually 1.  >1 if the data has been low pass filtered 
 *                   but not decimated. 
 * multiPassMode:    "Multi-pass" refers to a mode where data for multiple 
 *                   frequency spans is interleaved.
 *                   0    = not multi-pass data, 
 *                   1..4 = multi-pass, corresponding to Paragon gate array 
 *			    modes.
 *                   5    = future multi-pass mode.
 * multiPassDecimations: >0 = number of decimations included in the 
 *                       multi-pass data.
 ******************************************************************************
 */
typedef struct SDF_DATA_HDR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    UniqueDataPtr unique_record;
    char        dataTitle[DATA_TITLE_CHAR_SIZE];
    SHORTSIZ16  domain;            /* Domain enum */
    SHORTSIZ16  dataType;          /* Data_type enum */

#if SDF_REV >= 3
    SHORTSIZ16  num_of_pointsOld;
    SHORTSIZ16  last_valid_indexOld; 
#else
    SHORTSIZ16  num_of_points;
    SHORTSIZ16  last_valid_index; 
#endif /* SDF_REV >= 3 */

#if SDF_REV >= 2
    FLOATSIZ32  abscissa_firstXOld; /* Old, SUPPORT LIMITED */
    FLOATSIZ32  abscissa_deltaXOld; /* Old, SUPPORT LIMITED */
#else
    FLOATSIZ32  abscissa_firstX;
    FLOATSIZ32  abscissa_deltaX;
#endif /* SDF_REV >= 2 */

    SHORTSIZ16  xResolution_type;  /* XResolution_type enum */
    SHORTSIZ16  xdata_type;        /* Number_type enum */
    SHORTSIZ16  xPerPoint;
    SHORTSIZ16  ydata_type;        /* Number_type enum */
    SHORTSIZ16  yPerPoint;
    SHORTSIZ16  yIsComplex;
    SHORTSIZ16  yIsNormalized;
    SHORTSIZ16  yIsPowerData;
    SHORTSIZ16  yIsValid;
    LONGSIZ32   first_VECTOR_recordNum; 
    SHORTSIZ16  total_rows;
    SHORTSIZ16  total_cols;
    Sdf_UNIT    xUnit;
    SHORTSIZ16	yUnitValid;
    Sdf_UNIT    yUnit;

#if SDF_REV >= 2
    FLOATSIZ64  abscissa_firstX;
    FLOATSIZ64  abscissa_deltaX;
    SHORTSIZ16  scanData;
    SHORTSIZ16  windowApplied;
#endif /* SDF_REV >= 2 */

#if SDF_REV >= 3
    LONGSIZ32   num_of_points;
    LONGSIZ32   last_valid_index;
    SHORTSIZ16  overSampleFactor;  
    SHORTSIZ16  multiPassMode;
    SHORTSIZ16  multiPassDecimations;
#endif /* SDF_REV >= 3 */

} Sdf_DATA_HDR, PTR32 *Sdf_DATA_HDR_PTR;


/******************************************************************************
 * recordType=    SDF_VECTOR_TYPE
 * recordSize=    sizeof(struct SDF_VECTOR_HDR) in bytes
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record. Ignore this field during save/recall
 * the_CHANNEL_record[2]:  channels involved in this SDF_VECTOR_HDR
 * the_CHANNEL_record[ROW] = the row channel
 * the_CHANNEL_record[COL] = the column channel
 * pwrOfChan[2]: power of each channel in the_CHANNEL_record[]
 *               scaled by PWR_OF_CHAN_MULTIPLIER.
 * pwrOfChan[ROW]: power of the row channel scaled by PWR_OF_CHAN_MULTIPLIER
 * pwrOfChan[COL]: power of the col channel scaled by PWR_OF_CHAN_MULTIPLIER
 *
 * Example: ch15 (the 15th channel) is the response channel,
 *          ch2  (the 2nd channel) is the reference channel,
 *          frequency response measurement data of these two channels:
 *          the_CHANNEL_record[ROW] = 14
 *          the_CHANNEL_record[COL] = 1
 *          pwrOfChan[ROW]  = PWR_OF_CHAN(1) = 48
 *          pwrOfChan[COL]  = PWR_OF_CHAN(-1) = -48
 * Example: Auto power of ch2 (the 2nd channel)
 *          the_CHANNEL_record[ROW] = 1
 *          the_CHANNEL_record[COL] = INVALID_CHANNEL = -1
 *          pwrOfChan[ROW]  = PWR_OF_CHAN(2) = 96
 *          pwrOfChan[COL]  = 0
 ******************************************************************************
 */
typedef struct SDF_VECTOR_HDR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    UNIQUE_PTR  unique_record;
    SHORTSIZ16  the_CHANNEL_record[2];
    SHORTSIZ16  pwrOfChan[2];
} Sdf_VECTOR_HDR, PTR32 *Sdf_VECTOR_HDR_PTR;


/******************************************************************************
 * recordType=    SDF_CHANNEL_TYPE;
 * recordSize=    sizeof(struct SDF_CHANNEL_HDR) in bytes
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record.  Ignore this field during save/recall
 * channelLabel: Channel documentation (null terminated),
 *               TransducerId, Channel Setup doc, etc
 * moduleId:     Channel module Id (null terminated)
 * serialNum:    Channel serial number (null terminated)
 * weight:       Weighting function applied to input data (Weight enum)
 * delayOld:     old delay (use delay)
 * range:        in "dBVpk"
 * direction: 	 Transducer orientation (Direction enum)
 * pointNum:     Input point number of measurement
 * coupling :    0=DC_COUPLING, 1=AC_COUPLING (Coupling enum)
 * overloaded:   0=not overloaded, 1=overloaded (Overloaded enum)
 * intLabel:     Internal unit label which has max length=9 (null-terminated)
 * engUnit:      EngrUnit for this channel
 * int2engrUnit: Convert data from internal unit to engr unit.
 *               Divide the internal unit data by int2engrUnit to get
 *               engineering unit data (V/EU, pC/EU, etc.).
 * inputImpedance:  Input impedance
 *
 *                   SDF-2
 * channelAttribute: Attributes of a channel
 *                   0=NO_ATTRIB
 *                   1=TACH_ATTRIB
 *                   2=REFERENCE_ATTRIB
 *		     4=CLOCKWISE_ATTRIB				
 *                   8=DEMOD_ATTRIB
 * aliasProtected: 0=data was not alias protected, 1=alias protected
 * digital:        0=analog input channel, 1=digital input channel
 * channelScale:   See below.
 * channelOffset:  When ydata_type is short or long, the following
 *                 formulate will convert the data to volts:
 *                   Volts = channelOffset + (channelScale * Ydata)
 * gateBegin:      Gated sweep start time in seconds
 * gateEnd:        Gated sweep stop time in seconds
 * userDelay:      User specified input channel time delay (or line
 *                 length), in seconds.
 *    Two different uses of this field now exist:
 *
 *    1. Transducer location around the shaft for the rotational machine
 *       application.   
 *       The 'userDelay' of ADC input channel header keeps the angle around
 *       the shaft where the transducer is attached.  The angle is defined
 *       in degree from the positive x-axis (horzontal) in the direction of
 *       counter clockwise.  The field is valid only when the 'direction' 
 *       field is 'R_DIRECTION' (radial direction).
 *
 *   2.  Tach delay.
 *       The 'userDelay' field for the tach-pulse (or external trigger)
 *       channel keeps the time delay between time zero of ADC data and
 *       tach time data.  The value is in seconds.  This information is
 *       important to compute the absolute phase from the tach (or trigger)
 *       pulse. The field is used by Geoduck, Toolkit and Salsa time capture
 *       data.
 *
 *                 SDF-3
 * delay:          in "sec"
 * carrierFreq:    Carrier frequency for demodulated data.
 * channelNumber:  Zero based channel #
 * channelModule:  Zero based channel module
 ******************************************************************************
 */
typedef struct SDF_CHANNEL_HDR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    UniqueChanPtr unique_record;
    char        channelLabel[CHANNEL_LABEL_CHAR_SIZE];
    char        moduleId[MODULE_ID_CHAR_SIZE];
    char        serialNum[SERIAL_NUM_CHAR_SIZE];
    Sdf_WINDOW  window;
    SHORTSIZ16  weight;           /* Weight enum */

#if SDF_REV >= 3
    FLOATSIZ32  delayOld;
#else
    FLOATSIZ32  delay;
#endif

    FLOATSIZ32  range;
    SHORTSIZ16  direction;        /* Direction enum */
    SHORTSIZ16  pointNum;
    SHORTSIZ16  coupling;         /* Coupling enum */
    SHORTSIZ16  overloaded;       /* Overloaded enum */
    char        intLabel[INT_LABEL_CHAR_SIZE];
    Sdf_UNIT    engUnit;
    FLOATSIZ32  int2engrUnit;
    FLOATSIZ32  inputImpedance;

#if SDF_REV >= 2
    SHORTSIZ16  channelAttribute;  /* Attrib_type */
    SHORTSIZ16  aliasProtected;
    SHORTSIZ16  digital;
    FLOATSIZ64  channelScale;
    FLOATSIZ64  channelOffset;
    FLOATSIZ64  gateBegin;
    FLOATSIZ64  gateEnd;
    FLOATSIZ64  userDelay;
#endif /* SDF_REV >= 2 */

#if SDF_REV >= 3
    FLOATSIZ64  delay;
    FLOATSIZ64  carrierFreq;
    SHORTSIZ16  channelNumber;
    SHORTSIZ16  channelModule;
#endif /* SDF_REV >= 3 */

} Sdf_CHANNEL_HDR, PTR32 *Sdf_CHANNEL_HDR_PTR;


/*****************************************************************************
 * NOTE:                                                                     *
 * =====                                                                     *
 *  The SDF_SCAN_STRUCT is the old format and can only contain a scan        *
 *  count up to 32767 scans, even though the actual space allowed for        *
 *  scans is larger (recordSize is 32 bits).  For backwards compatibility,   *
 *  there will always be an SDF_SCAN_STRUCT in the SCAN_STRUCT area.         *
 *  If large scans are being used or multiple scan variables are used, then  *
 *  they are in the SCAN_BIG area.  The first record in this area will be    *
 *  an SDF_SCAN_BIG.  This record contains extended information on           *
 *  number of scans in the file.                                             *
 *  Notice that the scan variable has been split into a separate structure,  *
 *  SDF_SCAN_VAR.  After the SDF_SCAN_BIG record, there may be 0 to N        *
 *  SDF_SCAN_VAR records.  This is to allow multiple scan variables          *
 *  in a waterfall.  For example, this allows both time and RPM values       *
 *  to be stored as scan values in a file.                                   *
 *                                                                           *  
 *  The SDF_SCAN_STRUCT can contain the only scan variable information       *
 *  and since it can physically hold more that 32767 scans, then only        *
 *  the SDF_SCAN_BIG is needed (not SDF_SCAN_VAR records) to determine       *
 *  how many scans are in the scan variable area of the SDF_SCAN_STRUCT.     *
 *                                                                           *
 *  If backwards compatibility is not an issue, then the SDF_SCAN_STRUCT     *
 *  record can be deleted (num_of_SCAN_STRUCT_record = 0) and only the       *
 *  SDF_SCAN_BIG is specified (num_of_SCAN_BIG_record >= 1).                 *
 *  If no SDF_SCAN_VAR records are present, then the default scan            *
 *  variable will be "count" starting at 1 for the first scan.               *
 *****************************************************************************/


/******************************************************************************
 * recordType=  SDF_SCAN_STRUCT_TYPE
 * recordSize:  sizeof(struct SDF_SCAN_STRUCT) + total byte size of all
 *              scan data values in the entire file.
 * num_of_scan: 1 to n; number of depths or scans.
 *              All SDF_DATA_HDRs that are scan based (scanData = 1) have 
 *              to have the same num_of_scan scans.
 * last_scan_index: The last valid scan index (0 to num_of_scan-1).
 * scan_type:       0=DEPTH, 1=SCAN
 * scanVar_type:    1=SHORT16, 2=LONG32, 3=FLOAT32, 4=DOUBLE64 (Number_type)
 * scanUnit:        Unit for scan variables.
 *
 * For instance: 
 *    At three different times: 10sec, 20sec and 30sec, run meas.
 *        num_of_scan = 3
 *        last_scan_index = 2  (0 to num_of_scan-1)
 *        scan_type = DEPTH (in HP3566/67)
 *        scanVar_type = FLOAT32
 *        scanUnit = "sec"
 *        Scan Data: 10.0 20.0 30.0
 ******************************************************************************
 */
typedef struct SDF_SCAN_STRUCT {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    SHORTSIZ16  num_of_scan;
    SHORTSIZ16  last_scan_index;
    SHORTSIZ16  scan_type;           /* Scan_type enum */
    SHORTSIZ16  scanVar_type;        /* Number_type enum */
    Sdf_UNIT    scanUnit;
} Sdf_SCAN_STRUCT, PTR32 *Sdf_SCAN_STRUCT_PTR;


/******************************************************************************
 * recordType=  SDF_SCAN_BIG_TYPE
 * recordSize:  sizeof(struct SDF_SCAN_BIG)
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record.  Ignore this field during save/recall
 * num_of_scan: 1 to n; number of depths or scans.
 *              All SDF_DATA_HDRs that are scan based (scanData = 1) have 
 *              to have the same num_of_scan scans.
 * last_scan_index: The last valid scan index (0 to num_of_scan-1).
 * scan_type:       0=DEPTH, 1=SCAN
 ******************************************************************************
 */
typedef struct SDF_SCAN_BIG {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    LONGSIZ32   unique_record;
    LONGSIZ32   num_of_scan;
    LONGSIZ32   last_scan_index;
    SHORTSIZ16  scan_type;        /* Scan_type enum */
} Sdf_SCAN_BIG, PTR32 *Sdf_SCAN_BIG_PTR;


/******************************************************************************
 * recordType=  SDF_SCAN_VAR
 * recordSize:  sizeof(struct SDF_SCAN_VAR) + total byte size of all
 *              scan data values in the entire file.
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record.  Ignore this field during save/recall
 * headerSize:     sizeof(struct SDF_SCAN_VAR)
 * scanBase_type:  Type of scan data (e.g. time, rpm; ScanBase_type enum)
 * scanOrder_type: Order (+ or -) of scan values (ScanOrder_type enum)
 * DATA_recordNum: SDF_DATA_HDR # associated w/ this record
 *                 (-1 if no specific association).
 * scan_ID:        ID name (null-terminated) of scan information. 
 * scanVar_type:   1=SHORT16, 2=LONG32, 3=FLOAT32, 4=DOUBLE64 (Number_type)
 * scanUnit:       Unit for scan variables.
 ******************************************************************************
 */
typedef struct SDF_SCAN_VAR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    LONGSIZ32   unique_record;
    LONGSIZ32   headerSize;
    SHORTSIZ16  scanBase_type;      /* ScanBase_type enum */
    SHORTSIZ16  scanOrder_type;     /* ScanOrder_type enum */
    SHORTSIZ16  DATA_recordNum;
    char        scan_ID[SCAN_ID_CHAR_SIZE];
    SHORTSIZ16  scanVar_type;       /* Number_type enum */
    Sdf_UNIT scanUnit;
} Sdf_SCAN_VAR, PTR32 *Sdf_SCAN_VAR_PTR;


/******************************************************************************
 * recordType=  SDF_COMMENT_HDR
 * recordSize:  sizeof(struct SDF_COMMENT_HDR) + total byte size of the
 *              comment text area.
 * unique_record: If SDF is used as the internal data structure, this
 *                field contains the internal memory address of this record's 
 *                unique record.  Ignore this field during save/recall
 * headerSize:    sizeof(struct SDF_COMMENT_HDR)
 * comment_bytes: Size of comment (in bytes).  This size may be smaller than
 *                the comment text area.  If the size of the text is -1, then
 *                the entire comment text area is valid (or until an end
 *                of text marker is found).
 * comment_type:  Type of comment data (e.g. text; Comment_type enum)
 * scope_type:    To which type of header does this comment apply 
 *                (Scope_type enum)
 * scope_info:    The index of the header associated with scope_type
 *                (-1 = no specific header)
 ******************************************************************************
 */
typedef struct SDF_COMMENT_HDR {
    SHORTSIZ16  recordType;
    LONGSIZ32   recordSize;
    LONGSIZ32   unique_record;
    LONGSIZ32   headerSize;
    LONGSIZ32   comment_bytes;
    SHORTSIZ16  comment_type;     /* Comment_type enum */
    SHORTSIZ16  scope_type;       /* Scope_type enum */
    SHORTSIZ16  scope_info;
} Sdf_COMMENT_HDR, PTR32 *Sdf_COMMENT_HDR_PTR;


/*****************************************************************************
 * NOTE:                                                                     *
 * =====                                                                     *
 *  The only comment_type currently defined is COMMENT_TEXT.                 *
 *  COMMENT_TEXT contains ASCII text with LF (or CR/LF) at the end of each   *
 *  line of text.  Each instrument is free to to truncated length of lines   *
 *  and limit the total number of lines shown, but a guaranteed display      *
 *  minimum is line length of 40 characters and a length of 10 lines.        *
 *  It would be nice if instruments could plot this when plotting, but       *
 *  this is not intended to solve all plot annotation needs.                 *
 *  The COMMENT_TEXT area will contain only printing characters and          *
 *  CR/LF to allow for the use of multibyte character sets in the            *
 *  comment area.                                                            *
 *                                                                           *
 *  Possible additions to the comment_type might be sound data or graphical  *
 *  bit maps.                                                                *
 *****************************************************************************/


#endif /* SDF_comm_h */
