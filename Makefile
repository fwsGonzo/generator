OUTPUT = ../generator.dll
CC = c:\mingw64\bin\gcc
CFLAGS = -c -Iinc -std=gnu99 -pipe -m32 -Ofast -mfpmath=both
LFLAGS = -shared -Llib -std=gnu99 -pipe -m32 -Ofast -mfpmath=both

CMODS = generator.c std.c biome.c biomegen.c stdgen.c noise.c stdpp.c vec.c noise\cosnoise.c noise\multifractal.c noise\noise1234.c noise\simplex1234.c noise\torusnoise.c
COBJS = $(CMODS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

all: $(COBJS)
	$(CC) $(COBJS) $(LFLAGS) -o $(OUTPUT)

