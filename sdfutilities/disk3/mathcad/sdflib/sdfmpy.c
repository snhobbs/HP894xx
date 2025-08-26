/* $Header: sdfmpy.c,v 1.1 93/03/17 14:03:42 mikeh Exp $ */

/*
 * SDFMPY <file> <factor>
 *
 *         Demonstrates reading and writing to an sdf file by multiplying
 *         the first result (first row/col/scan) in the SDF <file> specified 
 *         by the numeric <factor> specified.
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


/*  Read the first row/column/scan of the Y Data for the result specified
 *  and multiply by the passed factor and write the data back to the
 *  file.
 */
void mpyYData (short result, double factor)
{
    Sdf_DATA_HDR dataHdr;     /* SDF data header */
    short row = 0, col = 0;
    long scan = 0L;
    int cmplxSize;
    long numberCnt, offsetCnt, xferCnt;

    /*  read the data header for the first data result
     */
    if (SdfDataHdr (fileNum, result, &dataHdr) )
	error ("SdfDataHdr failed\n");
 
    cmplxSize = (dataHdr.yIsComplex ? 2 : 1);

    /* number of data values to read.
     */
    numberCnt = dataHdr.last_valid_index + 1;
    offsetCnt = 0;

    /*  scale point cnt and offset by the # of values per point
     */
    numberCnt *= dataHdr.yPerPoint * cmplxSize;
    offsetCnt *= dataHdr.yPerPoint * cmplxSize;

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
	    error ("SdfYdata failed\n");

	/* multiply the data */
    	for (i = 0; i < xferCnt; ++i)
	    buffer[i] *= factor;

	/* write the data */
	if (SdfYdataWrite (fileNum, offsetCnt, result, row, col, 
		           scan, xferCnt, DOUBLE64, (float *) buffer) )
	    error ("SdfYdataWrite failed\n");

	offsetCnt += xferCnt;
	numberCnt -= xferCnt;
    }
}


/*  This is it
 */
int main (int argc, char *argv[])
{
    double mpyFactor;
    long truncateCnt;

    if (argc < 3)
	error ("usage: %s <filename> <factor>", argv[0] );

    mpyFactor = atof (argv[2]);
    if (mpyFactor == 0.0)
        error ("<factor> must not be zero");

    if ( (fileNum = SdfOpen (argv[1]) ) == 0)
	error ("SdfOpen of '%s' failed", argv[1] );

    /*  clear any previous truncation count */
    SdfTrunc (fileNum);

    /*  multiply data result 0 by mpyFactor
     */
    mpyYData (0, mpyFactor);

    /*  check to see if any data values were truncated when written back
     *  to the SDF file.  This could happen to time capture (integer) data
     *  if it was multiplied by a value > 1.0 and any values in the file
     *  were near the maximum range of an integer
     */
    truncateCnt = SdfTrunc (fileNum);
    if (truncateCnt != 0L)
        printf ("Warning: %ld data values were truncated\n", truncateCnt);

    if (SdfClose (fileNum) )
	error ("SdfClose failed");

    return 0;
}
