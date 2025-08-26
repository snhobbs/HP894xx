SDF Utilities Libraries Documentation


The SDF Utilities libraries support SDF programming with these languages:

    - Microsoft C 7.0


Files:

    sdf.c	 - Microsoft C 7.0 sample program to print headers
    sdfdata.c	 - Microsoft C 7.0 sample program to print data
    sdfmpy.c	 - Microsoft C 7.0 sample program to multiply data by constant
    sdfadd.c	 - Microsoft C 7.0 sample program to add constant to data
    sdf.mak 	 - Microsoft C 7.0 makefile
    sdf_basi.h	 - Microsoft C 7.0 include file
    sdf_comm.h	 - Microsoft C 7.0 include file
    sdf_util.h	 - Microsoft C 7.0 include file
    sdfutil.lib  - Microsoft C 7.0 large model library


To compile and run the sample program under Microsoft C 7.0:

    nmake -f sdf.mak
    sdf <sdf file>
    sdfdata <sdf file>
    sdfmpy <sdf file> <factor>
    sdfadd <sdf file> <factor> <result> <row> <col>
