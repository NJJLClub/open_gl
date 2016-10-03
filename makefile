MOTIF_IPATH = -I/usr/X11R6/include
MOTIF_LIB =-L/usr/X11R6/lib64

INCLUDES = $(MOTIF_IPATH)
LIBS = $(MOTIF_LIB)

Hello_World : Hello_World.c
	g++ -o Hello_World Hello_World.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm Hello_World
	
