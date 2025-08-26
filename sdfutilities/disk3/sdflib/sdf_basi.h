#ifndef SDF_basi_h
#define SDF_basi_h

/****************************************************************************
 * $Copyright: (C) 1988-1993, Hewlett-Packard Co. All rights reserved.$
 *
 * $Header: SDF_basi.h,v 1.53 94/06/03 22:41:20 hmgr Exp $
 *
 * $Module name: SDF_basi.h $
 * Product:      SDF shared w/ instruments, PCs, & workstations (s300, s700)
 *
 * $Revision: 1.53 $
 * $Date: 94/06/03 22:41:20 $
 * RCS $State: Exp $
 *
 * $Contributor: Lake Stevens Instrument Division $
 *
 * $Description: SDF basic types include file $
 *
 ****************************************************************************/


/* 
 * Standard Data Format revision 
 */
#ifndef SDF_REV
#define SDF_REV 3  /* default SDF revision */
#endif
#define CURRENT_REV SDF_REV /* for backward compatibility */

/* 
 * Always use a long pointer
 */
#ifdef MSDOS
#define PTR32 _far
#define HUGE32 _huge
#else
#define PTR32
#define HUGE32
#endif /* MSDOS */

/* 
 * used in set_unit() in SDF_def.c
 */ 
enum {
    UNSET_UNIT = 0,
    lbm_UNIT   = 101, /* lbm */
    in_UNIT,          /* in  */
    ft_UNIT,          /* ft  */
    s_UNIT,           /* s   */
    h_UNIT,           /* h   */
    min_UNIT,         /* min */
    lb_UNIT,          /* lb  */
    lbf_UNIT,         /* lbf */
    in_s_UNIT,        /* in/s */
    in_s_2_UNIT,      /* in/s^2 */
    g_UNIT,           /* g's */
    psi_UNIT,         /* psi */
    BTU_UNIT,         /* BTU */
    W_M_2_UNIT,       /* W/m^2 */
    BTU_h_ft_2_UNIT,  /* BTU/h*ft^2 */
    hz_UNIT,          /* hz */
    rpm_UNIT,         /* rpm */
    A_UNIT,           /* A   */
    W_UNIT,           /* W   */
    V_UNIT,           /* V   */
    Ohm_UNIT,         /* Ohm */
    pC_UNIT,          /* pC  */ 
    m_UNIT,           /* m   */
    m_s_UNIT,	      /* m/s */
    kgf_UNIT,	      /* kgf */
    SPL_UNIT,	      /* SPL */
    Pa_UNIT,          /* Pa  */
    rev_UNIT,         /* rev */
    ord_UNIT,         /* order */
    count_UNIT,       /* count */
    percent_UNIT      /* percent */
};

/* 
 * macros used in SDF_func.c and SDF_def.c 
 */
#define INVALID_OFFSET	             -1 
#define INVALID_CHANNEL              -1 
#define INVALID_ADDR                 0

#ifndef TRUE
#define TRUE                         1
#define FALSE                        0
#endif

#define PASSED                       1
#define FAILED                       0

/*
 * data types in any SDF structure
 */
#define SHORTSIZ16                   short  /* 16 bits */
#define USHORTSIZ16		     unsigned short /* 16 bits */
#define LONGSIZ32                    long   /* 32 bits */
#define FLOATSIZ32                   float  /* 32 bits */
#define UNIQUE_PTR                   long   /* 32 bits */
#define FLOATSIZ64                   double /* 64 bits */
#define VOID_PTR                     void PTR32 *
#define CHAR_PTR                     char PTR32 *

#define STREAM_PTR	             FILE *		

#define BYTE_SIZE_OF_SHORT		2
#define BYTE_SIZE_OF_LONG		4
#define BYTE_SIZE_OF_FLOAT		4
#define BYTE_SIZE_OF_DOUBLE		8

/* 
 * used in SDF_comm.h, etc 
 */
#define ROW			0
#define COL			1

#define UNIT_LABEL_CHAR_SIZE   10
#define APPLIC_VER_CHAR_SIZE   8
#define MEAS_TITLE_CHAR_SIZE   60
#define DATA_TITLE_CHAR_SIZE   16
#define CHANNEL_LABEL_CHAR_SIZE 	30
#define SERIAL_NUM_CHAR_SIZE   12
#define MODULE_ID_CHAR_SIZE    12
#define INT_LABEL_CHAR_SIZE    10
#define FORMAT_TYPE_CHAR_SIZE  2
#define SCAN_ID_CHAR_SIZE      10

/* 
 * SDF recordType 
 */
#define SDF_FIRST_COMMON       SDF_FILE_HDR_TYPE

#define SDF_FILE_HDR_TYPE      10   
#define SDF_MEAS_TYPE          11
#define SDF_DATA_TYPE          12
#define SDF_VECTOR_TYPE        13
#define SDF_CHANNEL_TYPE       14
#define SDF_SCAN_STRUCT_TYPE   15
#define SDF_XDATA_TYPE         16
#define SDF_YDATA_TYPE         17

#define SDF_SCAN_BIG_TYPE      18
#define SDF_SCAN_VAR_TYPE      19
#define SDF_COMMENT_TYPE       20

#define SDF_LAST_COMMON        SDF_COMMENT_TYPE

/* RESERVE 1000-2000 FOR INDIVIDUAL UNIQUE RECORD'S recordType */


#define SDF_MAX_FLOAT          ((FLOATSIZ32)3.0e+30)
#define CAST_IN_FLOAT(aDouble) \
        (aDouble > MAXFLOAT) ? SDF_MAX_FLOAT : (FLOATSIZ32)aDouble

#define PWR_OF_CHAN_MULTIPLIER	48
#define PWR_OF_CHAN(aValue) \
        (SHORTSIZ16)(PWR_OF_CHAN_MULTIPLIER*aValue)

#define PWR_OF_UNIT_MULTIPLIER	2
#define PWR_OF_UNIT(aDimension) \
	(SHORTSIZ16)(PWR_OF_UNIT_MULTIPLIER*aDimension)

#define FORMAT_IN_ASCII(format_struct_ptr) \
        format_struct_ptr->ascii_or_bin_char = 'A'; \
        format_struct_ptr->last_char = (char)0;
#define FORMAT_IN_BIN(format_struct_ptr) \
        format_struct_ptr->ascii_or_bin_char = 'B'; \
        format_struct_ptr->last_char = (char)0;
 

/* 
 * SDF_FILE_HDR applic 
 */
typedef enum 
{
    UNKNOWN_APPLIC = -99,
    HP35630        = -1,
    HP35631        = -2,
    HP35660        = -3,
    HP3562_63      = -4,
    HP3588         = -5,
    HP3589         = -6,
    HP3566_67      = 1,
    HP35665        = 2,
    HP3560         = 3,
    HP89410_440    = 4,
    PAGN_CAL       = 5,
    HP35635R       = 7,
    HP35654A_S1A   = 8,
    HP3569	   = 9,
    HP35670        = 10,
    HP3587         = 11,
    HPE3200        = 12,
    NUM_APPLIC	   = 13
} Applic;

/* 
 * SDF_MEAS_HDR averageType
 */
typedef enum 
{
    UNKNOWN_AVERAGE = -99,
    AVERAGE_OFF     = 0,
    RMS_AVERAGE,
    RMS_EXPO,
    VECTOR_AVERAGE,
    VECTOR_EXPO,
    CONT_PEAK_HOLD,
    PEAK_AVERAGE,
    TIME_AVERAGE,
    CONT_TIME_AVERAGE,
    TIME_EXPO_AVERAGE,
    EQUAL_CONF_AVERAGE,
    NUM_AVERAGE_TYPE
} Average_type;

/* 
 * SDF_MEAS_HDR measType 
 */
typedef enum 
{
    UNKNOWN_MEAS  = -99,
    SPECTRUM_MEAS = 0,
    NETWORK_MEAS,
    SWEPT_MEAS,
    FFT_MEAS,
    ORDERS_MEAS,
    OCTAVE_MEAS,
    CAPTURE_MEAS,
    CORRELATION_MEAS,
    HISTOGRAM_MEAS,
    SWEPT_NETWORK_MEAS,
    FFT_NETWORK_MEAS,
    SWEPT_FFT_MEAS,
    ANALOG_DEMOD_MEAS,
    DIGITAL_DEMOD_MEAS,
    NUM_MEASURE_TYPE
} Measure_type;

/* 
 * SDF_MEAS_HDR detection 
 */
typedef enum 
{
    UNKNOWN_DETECT  = -99,
    SAMPLE_DETECT = 0,
    POS_PEAK_DETECT,
    NEG_PEAK_DETECT,
    ROSENFELL_DETECT,
    NUM_DETECT_TYPE
} Detect_type;

/* 
 * SDF_DATA_HDR domain
 */
typedef enum 
{
    UNKNOWN_DOMAIN  = -99,
    FREQ_DOMAIN	= 0,
    TIME_DOMAIN,
    AMPLITUDE_DOMAIN,
    RPM_DOMAIN,
    ORDER_DOMAIN,
    CHANNEL_DOMAIN,
    OCTAVE_DOMAIN,
    NUM_DOMAIN
} Domain;

/* 
 * SDF_DATA_HDR dataType
 */
typedef enum
{
    UNKNOWN_DATA = -99,

    TIME_DATA    = 0,
	/* domain=TIME_DOMAIN, yPerPoint=1, yIsComplex=complex,
	   yIsPowerData=FALSE, yIsNormalized=FALSE */

    LINEAR_SPC_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=TRUE, 
	   yIsPowerData=FALSE, yIsNormalized=FALSE */

    AUTO_PWR_DATA,
	/* In Single Input case:
	     domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	     yIsPowerData=TRUE, yIsNormalized=FALSE
	   In MIMO case:
	     domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
             yIsPowerData=TRUE, yIsNormalized=FALSE */

    CROSS_PWR_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    FREQ_RESP_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=FALSE, yIsNormalized=FALSE */

    AUTO_CORREL_DATA,
	/* domain=TIME_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    CROSS_CORREL_DATA,
	/* domain=TIME_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    IMPL_RESP_DATA,
	/* domain=TIME_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=FALSE, yIsNormalized=FALSE */ 

    ORD_COH_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1 yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=TRUE */

    PART_COH_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1 yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=TRUE */

    MULT_COH_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1 yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=TRUE */

    FULL_OCTAVE_DATA,
	/* domain=OCTAVE_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    THIRD_OCTAVE_DATA,
	/* domain=OCTAVE_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    CONVOLUTION_DATA,
	/* domain=TIME_DOMAIN or FREQ_DOMAIN, yPerPoint=1, yIsComplex=TRUE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    HISTOGRAM_DATA,
	/* domain=AMPLITUDE_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=FALSE, yIsNormalized=FALSE */

    PRO_DENSITY_FUNC_DATA,  /* probability density func of active channels */
	/* domain=AMPLITUDE_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=FALSE, yIsNormalized=TRUE */

    CUM_DENSITY_FUNC_DATA,  /* cumulative density func of active channels */
	/* domain=AMPLITUDE_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=FALSE, yIsNormalized=TRUE */

    PWR_SPC_ORDER_TRACK_DATA,
	/* domain=RPM_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    COMP_PWR_TRACK_DATA,
	/* domain=RPM_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE*/

    PHASE_ORDER_TRACK_DATA,
	/* domain=RPM_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    RPM_SPECTRAL_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    ORDER_RATIO_DATA,
	/* domain=FREQ_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=TRUE, yIsNormalized=FALSE */

    ORBIT_DATA,
	/* domain=TIME_DOMAIN, yPerPoint=2, yIsComplex=optional,
	   yIsPowerData=optional, yIsNormalized=FALSE */

    PAGN_CAL_DATA,
	/* domain=UNKNOWN_DOMAIN, yPerPoint=1, yIsComplex=optional,
	   yIsPowerData=optional, yIsNormalized=FALSE */

    /*
     * HP3566/67 ADDITIONS
     */

    SINE_RMS_PWR_DATA,
	/* domain= FREQ_DOMAIN, yPerPoint=1, yIsComplex=FALSE,
	   yIsPowerData=FALSE, yIsNormalized=FALSE */

    SINE_VARIANCE_DATA,
    SINE_RANGE_DATA,
    SINE_SETTLE_TIME_DATA,
    SINE_INTEG_TIME_DATA,
    SINE_SOURCE_DATA,
    SINE_OVERLOAD_DATA,
    SINE_LINEAR_DATA,

    /*
     * SDF-2 ADDITIONS
     */

    SYNTHESIS,
    WEIGHT_DATA,
    FREQ_CORRECTION_DATA,
    ALL_PASS_TIME_DATA,
    NORM_REFERENCE_DATA,
    TACH_DATA,
    LIMIT_LINE_DATA,
    TWELFTH_OCTAVE_DATA,
    S11_DATA,
    S21_DATA,
    S12_DATA,
    S22_DATA,
    PSD_DATA,
   
    DECIMATED_TIME_DATA,
    OVERLOAD_DATA,
    COMPRESSED_TIME_DATA,

    EXTERNAL_TRIGGER_DATA, /* Toolkit - trigger mode of tach module */

    /*
     * Added for HP3569A Realtime Frequency Analyzer
     */

    PRESSURE_DATA,
    INTENSITY_DATA,
    PIINDEX_DATA,
    VELOCITY_DATA,
    PVINDEX_DATA,
    SOUND_PWR_DATA,
    FIELD_IND_DATA,
    PARTIAL_PWR_DATA,
    LN_01_DATA,
    LN_10_DATA,
    LN_50_DATA,
    LN_90_DATA,
    LN_99_DATA,
    LN_USER_DATA,
    T20_DATA,
    T30_DATA,
    RT60_DATA,
    AVG_CNT_DATA,
    SIGMA_DATA,


    /*
     * Added for HP89440 Vector Signal Analyzer Digital Demod Option
     */

    IQ_MEASURED_TIME_DATA,
    IQ_MEASURED_SPEC_DATA,
    IQ_REFERENCE_TIME_DATA,
    IQ_REFERENCE_SPEC_DATA,
    IQ_ERROR_MAG_DATA,
    IQ_ERROR_PHASE_DATA,
    IQ_ERROR_VECTOR_TIME_DATA,
    IQ_ERROR_VECTOR_SPEC_DATA,
    SYMBOL_TABLE_DATA,
 
    IQ_MEASURED_TIME_DATA_SUB_1,
    IQ_MEASURED_SPEC_DATA_SUB_1,
    IQ_REFERENCE_TIME_DATA_SUB_1,
    IQ_REFERENCE_SPEC_DATA_SUB_1,
    IQ_ERROR_MAG_DATA_SUB_1,
    IQ_ERROR_PHASE_DATA_SUB_1,
    IQ_ERROR_VECTOR_TIME_DATA_SUB_1,
    IQ_ERROR_VECTOR_SPEC_DATA_SUB_1,
    SYMBOL_TABLE_DATA_SUB_1,
  
    IQ_MEASURED_TIME_DATA_SUB_2,
    IQ_MEASURED_SPEC_DATA_SUB_2,
    IQ_REFERENCE_TIME_DATA_SUB_2,
    IQ_REFERENCE_SPEC_DATA_SUB_2,
    IQ_ERROR_MAG_DATA_SUB_2,
    IQ_ERROR_PHASE_DATA_SUB_2,
    IQ_ERROR_VECTOR_TIME_DATA_SUB_2,
    IQ_ERROR_VECTOR_SPEC_DATA_SUB_2,
    SYMBOL_TABLE_DATA_SUB_2,
  
    IQ_MEASURED_TIME_DATA_SUB_3,
    IQ_MEASURED_SPEC_DATA_SUB_3,
    IQ_REFERENCE_TIME_DATA_SUB_3,
    IQ_REFERENCE_SPEC_DATA_SUB_3,
    IQ_ERROR_MAG_DATA_SUB_3,
    IQ_ERROR_PHASE_DATA_SUB_3,
    IQ_ERROR_VECTOR_TIME_DATA_SUB_3,
    IQ_ERROR_VECTOR_SPEC_DATA_SUB_3,
    SYMBOL_TABLE_DATA_SUB_3,
  
    IQ_MEASURED_TIME_DATA_SUB_4,
    IQ_MEASURED_SPEC_DATA_SUB_4,
    IQ_REFERENCE_TIME_DATA_SUB_4,
    IQ_REFERENCE_SPEC_DATA_SUB_4,
    IQ_ERROR_MAG_DATA_SUB_4,
    IQ_ERROR_PHASE_DATA_SUB_4,
    IQ_ERROR_VECTOR_TIME_DATA_SUB_4,
    IQ_ERROR_VECTOR_SPEC_DATA_SUB_4,
    SYMBOL_TABLE_DATA_SUB_4,

    IQ_INPUT_TIME_DATA,

    ERROR_SYMBOL_TABLE_DATA,
    
    /* RESERVE dataType: 1000-2000 for user's special dataType */ 	

    NUM_DATA_TYPE
} Data_type;

/* 
 * String labels for SDF_DATA_HDR dataTitle
 */
#define LABEL_ALLCRTN_DATA              "Cal Data"
#define LABEL_FECRTN_DATA               "Cal Data"
#define LABEL_FErawchA_DATA             "Cal Data"
#define LABEL_FErawchAB_DATA            "Cal Data"
#define LABEL_FE2ndpadAB_DATA           "Cal Data"

#ifdef MADHATTER

#define LABEL_TIME_DATA                 "Time" 
#define LABEL_LINEAR_SPC_DATA           "Lin Spec"
#define LABEL_AUTO_PWR_DATA             "Pwr Spec"
#define LABEL_CROSS_PWR_DATA            "Cross Spec"
#define LABEL_FREQ_RESP_DATA            "Freq Resp"
#define LABEL_AUTO_CORREL_DATA          "Auto Corr"
#define LABEL_CROSS_CORREL_DATA         "Cross Corr"
#define LABEL_ORD_COH_YX_DATA           "Coherence"

#else

#define LABEL_TIME_DATA                 "Time Record" 
#define LABEL_LINEAR_SPC_DATA           "Linear Spectral"
#define LABEL_AUTO_PWR_DATA             "Auto Pwr Spec"
#define LABEL_CROSS_PWR_DATA            "Cross Pwr: Gyx"
#define LABEL_FREQ_RESP_DATA            "Freq Resp: Hyx"
#define LABEL_AUTO_CORREL_DATA          "Auto Correl"
#define LABEL_CROSS_CORREL_DATA         "Cross Correl"
#define LABEL_ORD_COH_YX_DATA           "Ord Coh: COHyx"

#endif /* MADHATTER */

#define LABEL_UNKNOWN_DATA              "Unknown Data"
#define LABEL_IMPL_RESP_DATA            "Impl Resp: hyx"
#define LABEL_ORD_COH_XX_DATA           "Ord Coh: COHxx"
#define	LABEL_PART_COH_DATA             "Partial Coh"
#define LABEL_MULT_COH_DATA             "Multiple Coh"
#define LABEL_FULL_OCTAVE_DATA          "Full Octave"
#define LABEL_THIRD_OCTAVE_DATA         "Third Octave"
#define LABEL_CONVOLUTION_DATA          "Convolution"
#define LABEL_HISTOGRAM_DATA            "Histogram"
#define LABEL_PRO_DENSITY_FUNC_DATA     "Pro Density Func"
#define LABEL_CUM_DENSITY_FUNC_DATA     "Cum Density Func"
#define LABEL_PWR_SPC_ORDER_TRACK_DATA  "Pwr Spc Order T"
#define	LABEL_COMP_PWR_TRACK_DATA       "Composite Power"
#define LABEL_PHASE_ORDER_TRACK_DATA    "Phase Order Trac"
#define LABEL_RPM_SPECTRAL_DATA         "RPM Spectral"  
#define LABEL_ORDER_RATIO_DATA          "Order Ration"
#define LABEL_ORBIT_DATA                "Orbit"
#define LABEL_PAGN_CAL_DATA             "Paragon Cal"
#define LABEL_SINE_RMS_PWR_DATA         "RMS Power" 
#define LABEL_SINE_VARIANCE_DATA        "Variance"
#define LABEL_SINE_RANGE_DATA           "Range"
#define LABEL_SINE_SETTLE_TIME_DATA     "Settle Time"
#define LABEL_SINE_INTEG_TIME_DATA      "Integrate Time"
#define LABEL_SINE_SOURCE_DATA          "Distortion" 
#define LABEL_SINE_OVERLOAD_DATA        "Percent Range"
#define LABEL_SINE_LINEAR_DATA          "Sine Linear"
#define LABEL_PSD_DATA                  "Pwr Spec Dens"

/*
 * SDF-2 ADDITIONS
 */

#define LABEL_SYNTHESIS			"Synthesis"
#define LABEL_WEIGHT_DATA		"Weight Function"
#define LABEL_FREQ_CORRECTION_DATA	"Freq Correction"
#define LABEL_ALL_PASS_TIME_DATA	"All Pass Time"
#define LABEL_NORM_REFERENCE_DATA	"Norm Reference"
#define LABEL_TACH_DATA			"Tachometer Data"
#define LABEL_LIMIT_LINE_DATA		"Limit Lines"
#define LABEL_TWELFTH_OCTAVE_DATA	"Twelfth Octave"
#define LABEL_S11_DATA			"S11"
#define LABEL_S21_DATA			"S21"
#define LABEL_S12_DATA			"S12"
#define LABEL_S22_DATA			"S22"

/*
 * Added for HP3569A Realtime Frequency Analyzer
 */

#define LABEL_PRESSURE_DATA		"Pressure"
#define LABEL_INTENSITY_DATA		"Intensity"
#define LABEL_PIINDEX_DATA		"PI Index"
#define LABEL_VELOCITY_DATA		"Velocity"
#define LABEL_PVINDEX_DATA		"PV Index"
#define LABEL_SOUND_PWR_DATA		"Sound Power"
#define LABEL_FIELD_IND_DATA		"Field Indicator"
#define LABEL_PARTIAL_PWR_DATA		"Partial Pwr"
#define LABEL_LN_01_DATA		"Ln 1"
#define LABEL_LN_10_DATA		"Ln 10"
#define LABEL_LN_50_DATA		"Ln 50"
#define LABEL_LN_90_DATA		"Ln 90"
#define LABEL_LN_99_DATA		"Ln 99"
#define LABEL_LN_USER_DATA		"Ln User"
#define LABEL_T20_DATA			"T20"
#define LABEL_T30_DATA			"T30"
#define LABEL_RT60_DATA 		"RT60"
#define LABEL_AVG_CNT_DATA		"Average Count"
#define LABEL_SIGMA_DATA		"Sigma"

/* 
 * SDF_DATA_HDR xResolution_type
 */
typedef enum
{
    LINEAR_RES,
    LOG_RES,
    ARB_XPMeasHdr,
    ARB_XPDataHdr,
    ARB_XPVectHdr,
    NUM_XRESOLUTION_TYPE,

    LINEAR_SWP = LINEAR_RES,
    LOG_SWP = LOG_RES,
    NUM_SWP_SPACING = NUM_XRESOLUTION_TYPE
} XResolution_type;

/* 
 * SDF_DATA_HDR xdata_type, ydata_type and SDF_SCAN_STRUCT scanVar_type
 */
typedef enum {
    SHORT16 = 1,
    LONG32,
    FLOAT32,
    DOUBLE64
} Number_type;

/* 
 * SDF_CHANNEL windowType 
 */
typedef enum 
{
    UNKNOWN_WINDOW_TYPE = -99,
    WINDOW_NOT_APPLIED  = 0,
    HANNING,
    FLAT_TOP,
    UNIFORM,
    FORCE,
    /* EXPONENTIAL = RESPONSE    */
    EXPONENTIAL,
    USER_WINDOW,
    HAMMING,
    P301,
    P310,
    KAISER_BESSEL,
    HARRIS,
    BLACKMAN,
    RESOLUTION_FILTER,
    CORREL_LEAD_LAG,
    CORREL_LAG,
    GATED,
    P400,
    NUM_WINDOW_TYPE
} Window_type;

/* 
 * SDF_CHANNEL_HDR wideBandCorr 
 */
#define HANNING_WIDE_CORR	1.63299316186
#define UNIFORM_WIDE_CORR       1.0
#define FLATTOP_WIDE_CORR  	2.39823509624
#define FORCE_WIDE_CORR         1.0
#define EXPONENTIAL_WIDE_CORR   1.0
#define P301_WIDE_CORR     	2.25493386468
#define P310_WIDE_CORR     	1.97707742746   
#define USER_WIDE_CORR          1.0
#define LEAD_LAG_WIDE_CORR      1.0
#define LEAD_WIDE_CORR          1.0
#define P400_WIDE_CORR          2.07867618

/* 
 * SDF_CHANNEL_HDR narrowBandCorr 
 */
#define HANNING_NARROW_CORR	2.0
#define UNIFORM_NARROW_CORR     1.0
#define FLATTOP_NARROW_CORR	4.6869145
#define FORCE_NARROW_CORR       1.0
#define EXPONENTIAL_NARROW_CORR 1.0
#define P301_NARROW_CORR  	4.1726444
#define P310_NARROW_CORR 	2.8108034
#define USER_NARROW_CORR        1.0
#define LEAD_LAG_NARROW_CORR    2.0
#define LEAD_NARROW_CORR        2.0
#define P400_NARROW_CORR        3.0939122

/* 
 * SDF_CHANNEL_HDR windowBandWidth 
 */
#define HANNING_BAND_WIDTH 	1.5
#define UNIFORM_BAND_WIDTH	1.0
#define FLATTOP_BAND_WIDTH	3.8193596
#define FORCE_BAND_WIDTH	1.0
#define EXPONENTIAL_BAND_WIDTH	1.0
#define P301_BAND_WIDTH		3.42416853439
#define P310_BAND_WIDTH		2.02121999058
#define USER_BAND_WIDTH         1.0
#define LEAD_LAG_BAND_WIDTH     2.0
#define LEAD_BAND_WIDTH         2.0
#define P400_BAND_WIDTH         2.21534968000

/* 
 * SDF_CHANNEL_HDR windowCorrMode 
 */
typedef enum 
{
    CORR_NOT_APPLIED,
    NARROW,
    WIDE,
    USER_CORR,
    NUM_WINDOW_CORR_MODE
} Window_corr_mode;

/* 
 * SDF_CHANNEL_HDR weight 
 */
typedef enum
{
    NO_WEIGHTING,
    A_WEIGHTING,
    B_WEIGHTING,
    C_WEIGHTING,
    NUM_WEIGHT
} Weight;

/* 
 * SDF_CHANNEL_HDR direction 
 *
 *   Value    Direction       Axis
 *
 *   0        No direction
 *   1        X               rectangular
 *   2        Y               rectangular               global,local
 *   3        Z               rectangular,cylindrical   global,local
 *   4        R               cylindrical,spherical     global,local
 *   5        T(Theta)        cylindrical,spherical     global,local
 *   6        R(Phi)          spherical                 
 *   7        Tx              rotation around X         global
 *   8        Ty              rotation around Y         global
 *   9        Tz              rotation around Z         global
 */
typedef enum
{
    NO_DIRECTION,
    X_DIRECTION,
    Y_DIRECTION,
    Z_DIRECTION,
    R_DIRECTION,
    T_DIRECTION,
    P_DIRECTION,
    TX_DIRECTION,
    TY_DIRECTION,
    TZ_DIRECTION,
    NUM_DIRECTION
} Direction;

/* 
 * SDF_CHANNEL_HDR coupling 
 */
typedef enum
{
    DC_COUPLING,
    AC_COUPLING,
    NUM_COUPLING
} Coupling;

/* 
 * SDF_CHANNEL_HDR overloaded 
 */
typedef enum
{
    UNKNOWN_OVERLOAD_STATE = -99,
    NOT_OVERLOADED         = 0,
    OVERLOADED_OCCUR,
    NUM_OVERLOADED
} Overloaded;

/* 
 * SDF_CHANNEL_HDR channelAttribute (Attrib_type) 
 */
#define UNKNOWN_ATTRIB			-99
#define NO_ATTRIB 			0
#define TACH_ATTRIB 			1
#define REFERENCE_ATTRIB 		2
#define TACH_AND_REFERENCE_ATTRIB 	(TACH_ATTRIB+REFERENCE_ATTRIB)
#define CLOCKWISE_ATTRIB		4
#define DEMOD_ATTRIB                    8

/* 
 * SDF_SCAN_STRUCT scan_type 
 */
typedef enum 
{
    DEPTH,
    SCAN,
    NUM_SCAN_TYPE
} Scan_type;

/*
 * SDF_SCAN_VAR scanBase_type
 */
typedef enum 
{
    SCAN_UNKNOWN, 
    SCAN_NUMBER, 
    SCAN_TIME, 
    SCAN_RPM,
    SCAN_TEMPERATURE, 
    SCAN_TACH_CNT,
    NUM_SCANBASE_TYPE
} ScanBase_type;

/*
 * SDF_SCAN_VAR scanOrder_type
 */
typedef enum 
{
    SCAN_ORDER_UNKNOWN, 
    SCAN_ORDER_INCREASING, 
    SCAN_ORDER_DECREASING,
    NUM_SCANORDER_TYPE
} ScanOrder_type;


/*
 * SDF_COMMENT_HDR comment_type
 */
typedef enum 
{
    COMMENT_TEXT,
    NUM_COMMENT_TYPE
} Comment_type;


/*
 * SDF_COMMENT_HDR scope_type
 */
typedef enum 
{
    SCOPE_FILE, 
    SCOPE_DATA, 
    SCOPE_VECTOR, 
    SCOPE_CHANNEL,
    SCOPE_SCAN,
    NUM_SCOPE_TYPE
} Scope_type;


/*
 * Some projects want to define unique_record fields to be ptrs to structs,
 * rather than just UNIQUE_PTRs (long).  The actual uniqueXXX structs
 * are defined in project-specific SDF header files.
 */
#ifdef UNIQUE_AS_STRUCT_PTR
  typedef struct uniqueData PTR32 *UniqueDataPtr;
  typedef struct uniqueChan PTR32 *UniqueChanPtr;
#else
  typedef UNIQUE_PTR UniqueDataPtr;
  typedef UNIQUE_PTR UniqueChanPtr;
#endif


/*
 * DEFAULT VALUES USED IN SDF_def.c 
 */

/* 
 * default_meas_hdr() 
 */
#define DEF_CENTER_FREQ		32768.0
#define DEF_SPAN_FREQ		65536.0
#define DEF_BLOCK_SIZE		1024	
#define DEF_ZOOM_MODE_ON	FALSE
#define DEF_START_FREQ_INDEX    0
#define DEF_STOP_FREQ_INDEX     400
#define DEF_AVERAGE_TYPE	AVERAGE_OFF
#define DEF_AVERAGE_NUM		10
#define DEF_PCT_OVERLAP		0.0
#define DEF_VIDEO_BAND_WIDTH    0.0
/*
 * SDF-2 ADDITIONS
 */
#define DEF_SWEEP_FREQ          0.0
#define DEF_MEAS_TYPE           UNKNOWN_MEAS
#define DEF_REAL_TIME           FALSE
#define DEF_DETECTION           UNKNOWN_DETECT
#define DEF_SWEEP_TIME          0.0

/* 
 * default_data_hdr() 
 */ 
#define DEF_DATA_TITLE	        LABEL_LINEAR_SPC_DATA	
#define DEF_DOMAIN		FREQ_DOMAIN
#define DEF_DATA_TYPE		LINEAR_SPC_DATA
#define DEF_NUM_OF_POINTS	513
#define DEF_LAST_VALID_INDEX    512
#define DEF_ABS_FIRSTX		0.0
#define DEF_ABS_DELTAX		128.0
#define DEF_XRES_TYPE		LINEAR_RES
#define DEF_XDATA_TYPE		FLOAT32
#define DEF_X_PER_POINT		1
#define DEF_YDATA_TYPE		FLOAT32
#define DEF_Y_PER_POINT		1
#define DEF_Y_IS_COMPLEX	TRUE
#define DEF_Y_IS_NORMALIZED	FALSE
#define DEF_Y_IS_PWR_DATA	FALSE
#define DEF_Y_IS_VALID		TRUE
#define DEF_FIRST_VECTOR_RECORD	0
#define DEF_TOTAL_ROWS		1
#define DEF_TOTAL_COLS		1
#define DEF_Y_UNIT_VALID	FALSE
/* xUnit's label = "hz" */

/*
 * SDF-2 ADDITIONS
 */
#define DEF_SCAN_DATA           TRUE
#define DEF_WINDOW_APPLIED      TRUE

/*
 * SDF-3 ADDITIONS
 */
#define DEF_OVER_SAMPLE_FACTOR  1
#define DEF_MULTI_PASS_MODE     0
#define DEF_MULTI_PASS_DEC      0

/* 
 * default_vector_hdr() 
 */
#define DEF_CHANNEL_0		0
#define DEF_PWR_OF_CHAN_0	PWR_OF_CHAN(1)

/* 
 * default_channel_hdr() 
 */
#define DEF_WINDOW_TYPE		HANNING
#define DEF_WINDOW_CORR_MODE	WINDOW_NOT_APPLIED
#define DEF_WINDOW_BAND_WIDTH	1.0
#define DEF_WINDOW_TIME_CONST	1.0
#define DEF_WINDOW_TRUNC	1.0
#define DEF_WIDE_BAND_CORR	1.5
#define DEF_NARROW_BAND_CORR	1.0
#define DEF_WEIGHTING		NO_WEIGHTING
#define DEF_DELAY		0.0
#define DEF_RANGE		0.0
#define DEF_DIRECTION		NO_DIRECTION
#define DEF_POINT_NUM		0
#define DEF_COUPLING	        DC_COUPLING	
#define DEF_CHAN_INTLABEL	"V"
/* engrUnit;s label = "V" */
#define DEF_INT2ENGR		1.0 
#define DEF_INPUT_IMPEDANCE	1.0e6
#define DEF_OVERLOADED          UNKNOWN_OVERLOAD_STATE
/*
 * SDF-2 ADDITIONS
 */
#define DEF_CHANNEL_ATTRIBUTE   NO_ATTRIB
#define DEF_ALIAS_PROTECTED     TRUE
#define DEF_DIGITAL             FALSE
#define DEF_CHANNEL_SCALE       1.0
#define DEF_CHANNEL_OFFSET      0.0
#define DEF_GATE_BEGIN          0.0
#define DEF_GATE_END            0.0
#define DEF_USER_DELAY          0.0
/*
 * SDF-3 ADDITIONS
 */
#define DEF_CARRIER_FREQ        0.0
#define DEF_CHANNEL_NUMBER      0
#define DEF_CHANNEL_MODULE      0

/* 
 * default_scan_struct() 
 */
#define DEF_NUM_OF_SCAN		10
#define DEF_LAST_SCAN_INDEX     9
#define DEF_SCAN_TYPE		DEPTH
#define DEF_SCAN_VAR_TYPE	FLOAT32
/* scanUnit's label = "rpm" */

/* 
 * default_scan_var() 
 */
#define DEF_SCAN_BASE		SCAN_UNKNOWN
#define DEF_SCAN_ORDER		SCAN_ORDER_UNKNOWN
#define DEF_SCAN_RECORD		-1

/* 
 * default_comment_hdr() 
 */
#define DEF_COMMENT_BYTES	0
#define DEF_COMMENT_TYPE	COMMENT_TEXT
#define DEF_SCOPE_TYPE		SCOPE_FILE
#define DEF_SCOPE_INFO		-1


#endif /* SDF_basi_h */
