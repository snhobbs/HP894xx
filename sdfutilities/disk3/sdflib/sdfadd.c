/* $Header: sdfadd.c,v 1.1 93/04/20 10:43:52 mikeh Exp $ */

/*
 * SDFADD <filename> <factor> <result> <row> <col>", 
 *
 *         Demonstrates reading and writing to an sdf file by adding
 *         the numeric <factor> specified to all scans of the result/row/col
 *         specified in the SDF <filename>.
 *
 * Copyright 1993 Hewlett-Packard Company
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <SDF_comm.h>
#include <SDF_util.h>


#ifndef TRUE
#define FALSE 0
#define TRUE (~FALSE)
#endif


/* Global variables 
 */
short fileNum;			 /* file number used by SDF library */


/*  Generate error message and exit program
 */
void error (char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    vfprintf (stderr, fmt, args);
    fprintf (stderr, "\n");
    va_end (args);
    if (fileNum)
        SdfClose (fileNum);
    exit (-1);
}


/*  Read the specified row/column/scan of the Y Data for the result specified
 *  and add the passed factor and write the data back to the file.
 */
void addYDataScan (Sdf_DATA_HDR *dataPtr, short result, short row, short col, 
                   long scan, double factor)
{
    int cmplxSize;
    long numberCnt, offsetCnt, xferCnt;

    cmplxSize = (dataPtr->yIsComplex ? 2 : 1);

    /* number of data values to read.
     */
    numberCnt = dataPtr->last_valid_index + 1;
    offsetCnt = 0;

    /*  scale point cnt and offset by the # of values per point
     */
    numberCnt *= dataPtr->yPerPoint * cmplxSize;
    offsetCnt *= dataPtr->yPerPoint * cmplxSize;

    while (numberCnt > 0) 
    {
	/* read as many numbers that will fit into an input buffer
	 */
#define	BUF_SIZ 256
	double buffer[BUF_SIZ];
	long i;

	if ( (xferCnt = BUF_SIZ) > numberCnt)
	    xferCnt = numberCnt;

	/* read the data */
	if (SdfYdata (fileNum, offsetCnt, result, row, col, 
		      scan, xferCnt, DOUBLE64, (float *) buffer) )
	    error ("SdfYdata failed, result %d, row %d, col %d\n",
	            result, row, col);

	/* add to the real part of the data */
    	for (i = 0; i < xferCnt; ++i)
	    if ( ! dataPtr->yIsComplex || ! (i & 1) )
	        buffer[i] += factor;

	/* write the data */
	if (SdfYdataWrite (fileNum, offsetCnt, result, row, col, 
		           scan, xferCnt, DOUBLE64, (float *) buffer) )
	    error ("SdfYdataWrite failed\n");

	offsetCnt += xferCnt;
	numberCnt -= xferCnt;
    }
}


/*  Read the specified row/column/scan of the Y Data for the result specified
 *  and add the passed factor and write the data back to the
 *  file.
 */
void addYData (short result, short row, short col, double factor)
{
    Sdf_DATA_HDR dataHdr;     /* SDF data header */
    long scan, scans = 1;

    /*  read the data header for the first data result
     */
    if (SdfDataHdr (fileNum, result, &dataHdr) )
	error ("SdfDataHdr failed, result %d\n", result);
 
    /*  find out how many scans are in the data
     */
    if (dataHdr.scanData)
        scans = SdfScansUsed (fileNum);

    /*  Add to all scans in the data
     */
    for (scan = 0; scan < scans; ++scan)
        addYDataScan (&dataHdr, result, row, col, scan, factor);
}


/*  This is it
 */
int main (int argc, char *argv[])
{
    double addFactor;
    long truncateCnt;
    short result = 0, row = 0, col = 0;

    if (argc < 3)
	error ("usage: %s <filename> <factor> <result> <row> <col>", 
	       argv[0] );

    addFactor = atof (argv[2]);
    if (addFactor == 0.0)
        error ("<factor> must not be zero");

    if (argc >= 4)
        result = atoi (argv[3]);
    if (argc >= 5)
        row = atoi (argv[4]);
    if (argc >= 6)
        col = atoi (argv[5]);

    if ( (fileNum = SdfOpen (argv[1]) ) == 0)
	error ("SdfOpen of '%s' failed", argv[1] );

    /*  clear any previous truncation count */
    SdfTrunc (fileNum);

    /*  add data result to addFactor
     */
    addYData (result, row, col, addFactor);

    /*  Check to see if any data values were truncated when written back
     *  to the SDF file.  This could happen to time capture (integer) data
     *  if a large value was added and any values in the file
     *  were near the maximum range of an integer.
     */
    truncateCnt = SdfTrunc (fileNum);
    if (truncateCnt != 0L)
        printf ("Warning: %ld data values were truncated\n", truncateCnt);

    if (SdfClose (fileNum) )
	error ("SdfClose failed");

    return 0;
}
