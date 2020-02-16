INCLUDE=.;d:\ibmc\include

demodd.sys: demodd.obj
        link386 /nod /noe /m demodd, demodd.sys, demodd, vdh os2386, demodd.def
        mapsym demodd

demodd.obj: demodd.c demodd.h mvdm.h
        icc -Fa -Ss  -c -Gr -Rn -I$(INCLUDE) demodd.c

