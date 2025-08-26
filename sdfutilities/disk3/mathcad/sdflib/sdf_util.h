/* $Header: SDF_util.h,v 1.15 94/05/24 10:23:07 hmgr Exp $ */

/*
 * Copyright 1989 Hewlett-Packard Company
 */

#ifdef DLL
#define PROC FAR PASCAL _export
#else
#define PROC 
#endif /* DLL */

#define SHORTP short PROC
#define INTP int PROC
#define LONGP long PROC
#define VOIDP void PROC

/*
 * Error codes returned by SdfOpen() and SdfUpdateOpen()
 */
#define SDF_ERR_ERROR            0
#define SDF_ERR_NOT_EXIST       -1
#define SDF_ERR_WRITE_PROTECT   -2
#define SDF_ERR_NAME_TOO_LONG   -3
#define SDF_ERR_NOT_SDF         -4
#define SDF_ERR_TOO_MANY_FILES  -5

/*
 * Access SDF file
 */
SHORTP SdfUpdateOpen (CHAR_PTR filename);
SHORTP SdfOpen (CHAR_PTR filename);
SHORTP SdfMemOpen (CHAR_PTR memStart, long memLength);
INTP SdfClose (short fn);
LONGP SdfPos (short fn);
INTP SdfDefaultHdr (Sdf_HDR_PTR hdr, short recordType);
INTP SdfFileHdr (short fn, Sdf_FILE_HDR_PTR fileHdr);
INTP SdfMeasHdr (short fn, Sdf_MEAS_HDR_PTR measHdr);
INTP SdfDataHdr (short fn, short dataNo, Sdf_DATA_HDR_PTR dataPtr);
INTP SdfVectHdr (short fn, short dataNo, short rowNo, short colNo, 
	        Sdf_VECTOR_HDR_PTR vectHdr);
INTP SdfChanHdr (short fn, short chanNo, Sdf_CHANNEL_HDR_PTR chanHdr);

INTP SdfUniqHdr (short fn, VOID_PTR uniqHdr);

#if SDF_REV >= 3

INTP SdfCommentHdr (short fn, short commNo, Sdf_COMMENT_HDR_PTR commHdr);
LONGP SdfCommentRead (short fn, short commNo, long offset, long size,
                     char far *buf);
LONGP SdfCommentWrite (short fn, short commNo, long offset, long size,
                      char far *buf);

#endif /* SDF_REV >= 3 */

INTP SdfScanBig (short fn, Sdf_SCAN_BIG_PTR scanBig);
INTP SdfScanVar (short fn, short varInx, Sdf_SCAN_VAR_PTR scanVar);
INTP SdfScanStruct (short fn, Sdf_SCAN_STRUCT_PTR scanStruct);
LONGP SdfScansUsed (short fn);
INTP SdfYdataScale (Sdf_DATA_HDR_PTR dataPtr, 
		    Sdf_VECTOR_HDR_PTR vectPtr,
		    Sdf_CHANNEL_HDR_PTR rowChanPtr,
		    Sdf_CHANNEL_HDR_PTR colChanPtr,
                    double far *pCorr,
                    double far *pFltScale, double far *pFltOffset);
INTP SdfYdata (short fn, long offset, short dataNo, short rowNo, 
	       short colNo, long scanNo, long pnts, short dataType, 
	       float far *y);
INTP SdfYdataWrite (short fn, long offset, short dataNo, short rowNo, 
	            short colNo, long scanNo, long pnts, short dataType, 
		    float far *y);
INTP SdfXdata (short fn, long offset, short dataNo, short rowNo, 
	       short colNo, long pnts, short dataType, float far *x);
INTP SdfScanData (short fn, long offset, short varInx, 
                 long pnts, short dataType, float far *s);
LONGP SdfTrunc (short fn);

INTP SdfByteSizeOfDataType (short dataType);

INTP SdfRevision (void);
VOIDP SdfByteSwapHdr (Sdf_HDR_PTR hdr, short recordType);
VOIDP SdfByteSwapShort (short far *val);
VOIDP SdfByteSwapLong (long far *val);
VOIDP SdfByteSwapFloat (float far *val);
VOIDP SdfByteSwapDouble (double far *val);
VOIDP SdfSetUnit (Sdf_UNIT_PTR sdfUnit, short unitType);

#ifdef DLL
/*
 * Return pointer to SDF Header into SDF file in memory 
 */
Sdf_MEAS_HDR_PTR PROC SdfToMeasHdr (Sdf_file_PTR fd);
Sdf_DATA_HDR_PTR PROC SdfToDataHdr (Sdf_file_PTR fd, short dataNo);
Sdf_VECTOR_HDR_PTR PROC SdfToVectHdr2 (Sdf_file_PTR fd, short vectNo);
Sdf_CHANNEL_HDR_PTR PROC SdfToChanHdr (Sdf_file_PTR fd, short chanNo);
Sdf_SCAN_STRUCT_PTR PROC SdfToScanStruct (Sdf_file_PTR fd);
Sdf_HDR_PTR PROC SdfToYhdr (Sdf_file_PTR fd);
Sdf_HDR_PTR PROC SdfToXhdr (Sdf_file_PTR fd);
Sdf_COMMENT_HDR_PTR PROC SdfToCommentHdr (Sdf_file_PTR fd, short inx);
Sdf_HDR_PTR PROC SdfToScanBig (Sdf_file_PTR fd, short inx);
Sdf_HDR_PTR PROC SdfToUniqHdr (Sdf_file_PTR fd, short inx);

#endif

#ifndef DLL
int opensdf (char *filename); 
int closesdf (short *fn);
int filehdr (short *fn, Sdf_FILE_HDR_PTR fileHdr);
int meashdr (short *fn, Sdf_MEAS_HDR_PTR measHdr);
int datahdr (short *fn, short *dataNo, Sdf_DATA_HDR_PTR dataPtr);
int vecthdr (short *fn, short *dataNo, short *rowNo, short *colNo, 
	     Sdf_VECTOR_HDR_PTR vectHdr);
int chanhdr (short *fn, short *chanNo, Sdf_CHANNEL_HDR_PTR chanHdr);

#if SDF_REV >= 3

int commenthdr (short *fn, short *commNo, Sdf_COMMENT_HDR_PTR commHdr);
long commentread (short *fn, short *commNo, long *pOffset, long *pSize,
                  char far *buf);
long commentwrite (short *fn, short *commNo, long *pOffset, long *pSize,
                   char far *buf);

#endif /* SDF_REV >= 3 */

int scanbig (short *fn, Sdf_SCAN_BIG_PTR scanBig);
int scanvar (short *fn, short *scanVarInx, Sdf_SCAN_VAR_PTR scanVar);
int scanstruct (short *fn, Sdf_SCAN_STRUCT_PTR scanStruct);
long scansused (short *fn);
int yldataoff (short *fn, long *offset, short *dataNo, short *rowNo, 
	       short *colNo, long *scanNo, long *size, float far *y);
int ydata (short *fn, short *dataNo, short *rowNo, short *colNo, short *scanNo,
	   short *size, float far *y);
int ylwriteoff (short *fn, long *offset, short *dataNo, short *rowNo, 
	        short *colNo, long *scanNo, long *size, float far *y);
int ydataoff (short *fn, long *offset, short *dataNo, short *rowNo, 
	      short *colNo, short *scanNo, short *size, float far *y);
int xldataoff (short *fn, long *offset, short *dataNo, short *rowNo, 
	       short *colNo, long *size, float far *x);
int xdataoff (short *fn, long *offset, short *dataNo, short *rowNo, 
	      short *colNo, short *size, float far *x);
int xdata (short *fn, short *dataNo, short *rowNo, short *colNo, 
	   short *size, float far *x);
int scanvardata (short *fn, long *offset, short *varInx,
                 long *size, float far *s);
int scandataoff (short *fn, long *offset, short *size, float far *s);
int scandata (short *fn, short *size, float far *s);

#endif /* ! DLL */
