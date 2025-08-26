CFLAGS = /AL /Gs /Os /W3 /I.

all : sdf.exe sdfdata.exe sdfmpy.exe sdfadd.exe

sdf.exe : sdf.c
	cl $(CFLAGS) sdf.c -link sdfutil

sdfdata.exe : sdfdata.c
	cl $(CFLAGS) sdfdata.c -link /ST:8192 sdfutil

sdfmpy.exe : sdfmpy.c
	cl $(CFLAGS) sdfmpy.c -link /ST:8192 sdfutil

sdfadd.exe : sdfadd.c
	cl $(CFLAGS) sdfadd.c -link /ST:8192 sdfutil

