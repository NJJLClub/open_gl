MOTIF_IPATH = -I/usr/X11R6/include
MOTIF_LIB =-L/usr/X11R6/lib64

INCLUDES = $(MOTIF_IPATH)
LIBS = $(MOTIF_LIB)

gl_planets : gl_planets.c
	g++ -o gl_planets gl_planets.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm gl_planets
