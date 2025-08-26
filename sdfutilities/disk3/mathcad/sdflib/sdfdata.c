/* $Header: sdfdata.c,v 1.3 93/03/17 14:03:19 mikeh Exp $ */

/*
 * SDFDATA <file> [a]
 *
 *         Print all the Y Data in the SDF <file> specified.
 *         If 'a' is specified, then all lines of a spectrum will
 *         be printed (instead of just the alias protected region).
 *
 * Copyright 1992 Hewlett-Packard Company
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
Sdf_FILE_HDR fileHdr;  		 /* SDF file header */
Sdf_MEAS_HDR measHdr;     	 /* SDF measurement header */
short fileNum;			 /* file number used by SDF library */
long maxScans = 1;               /* Max # of scans used by any result */


/*  OPTIONS
 */
int allLines = FALSE;            /* Set this to true to print the data that
				  * is not alias protected.
				  */


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


/*  Read and print all the Y Data for the result specified
 *  (i.e. all scans, rows, columns).  Data is printed one value
 *  per line, so one point (real / imaginary pair) is split across
 *  multiple lines.  All the frequency lines are printed for frequency
 *  domain data only if the 'allLines' variable is set TRUE (this includes 
 *  the data that is not alias protected).
 *  Integer (capture) data is automatically scaled.
 */
void readYData (short result)
{
    Sdf_DATA_HDR dataHdr;     /* SDF data header */
    short row, col;
    int cmplxSize;
    long scan, numScans = 1;

    /*  read the data header associated with the data result
     */
    if (SdfDataHdr (fileNum, result, &dataHdr) )
	error ("SdfDataHdr failed, result %d", result);
 
    cmplxSize = (dataHdr.yIsComplex ? 2 : 1);

    /*  if this result has multiple scans read in the scan header
     *  to find out how many scans there are in this result.
     */
    if (dataHdr.scanData)
	numScans = maxScans;

    /* loop through all the scans in this result 
     */
    for (scan = 0; scan < numScans; scan++)
    {
	/* and all the columns 
	 */
        for (col = 0; col < dataHdr.total_cols; col++)
        {
	    /* and all the rows 
	     */
	    for (row = 0; row < dataHdr.total_rows; row++)
	    {
		long numberCnt, offsetCnt, xferCnt;

		/* number of data values to read.
		 */
		if (dataHdr.domain == FREQ_DOMAIN &&  ! allLines) 
		{   /* exclude the data that is not alias protected
		     */
		    numberCnt = measHdr.stopFreqIndex + 1 -
				measHdr.startFreqIndex;
		    offsetCnt = measHdr.startFreqIndex;
		}
		else 
		{   /* use all the data
		     */
		    numberCnt = dataHdr.last_valid_index + 1;
		    offsetCnt = 0;
		}

		/*  scale point cnt and offset by the # of values per point
		 */
		numberCnt *= dataHdr.yPerPoint * cmplxSize;
		offsetCnt *= dataHdr.yPerPoint * cmplxSize;

		printf ("Y DATA for RESULT %d", result);
		if (numScans > 1)
		    printf (", SCAN %ld", scan);
		if (dataHdr.total_cols > 1)
		    printf (", COL %d", col);
		if (dataHdr.total_rows > 1)
		    printf (", ROW %d", row);
		printf ("\n");

		while (numberCnt > 0) 
		{
		    /* read as many numbers that will fit into an input buffer
		     */
#define		    BUF_SIZ 128
		    double buffer[BUF_SIZ];
		    long i;

		    if ( (xferCnt = BUF_SIZ) > numberCnt)
			xferCnt = numberCnt;
		    if (SdfYdata (fileNum, offsetCnt, result, row, col, 
				  scan, xferCnt, DOUBLE64, (float *) buffer) )
		        error ("SdfYdata failed, res %d", result);

		    /* print the data */
    		    for (i = 0; i < xferCnt; ++i)
			printf ("%.10g\n", buffer[i]);

		    offsetCnt += xferCnt;
		    numberCnt -= xferCnt;
		}
	    }
	}
    }
}


/*  This is it
 */
int main (int argc, char *argv[])
{
    short result;

    if (argc < 2)
	error ("usage: %s <filename> [a]", argv[0] );

    if (argc >= 3 && toupper (*argv[2]) == 'A')
	allLines = TRUE;

    if ( (fileNum = SdfOpen (argv[1]) ) == 0)
	error ("SdfOpen of '%s' failed", argv[1] );

    /*  read File Header
     */
    if (SdfFileHdr (fileNum, &fileHdr) )
	error ("SdfFileHdr failed");

    /*  read Measurement Header 
     */
    if (SdfMeasHdr (fileNum, &measHdr) )
	error ("SdfMeasHdr failed");

    /*  read the Scan Header if there is one in this file and determine
     *  the maximum # of scans that any result can have
     */
    if ( (maxScans = SdfScansUsed (fileNum) ) < 0)
	error ("SdfScansUsed failed");
    if (maxScans < 1)
	maxScans = 1;

    /*  read and print all the Y data 
     */
    for (result = 0; result < fileHdr.num_of_DATA_HDR_record; result++)
	readYData (result);

    if (SdfClose (fileNum) )
	error ("SdfClose failed");

    return 0;
}
