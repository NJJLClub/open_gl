MOTIF_IPATH = -I/usr/X11R6/include
MOTIF_LIB =-L/usr/X11R6/lib64

INCLUDES = $(MOTIF_IPATH)
LIBS = $(MOTIF_LIB)

gl3d_draw : gl3d_draw.c
	g++ -o gl3d_draw gl3d_draw.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm gl3d_draw
	
