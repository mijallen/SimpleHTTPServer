# Makefile
#

CSOURCE = httpServer.c
OBJECTS = $(patsubst %.c,%.o,$(CSOURCE))
LIBRARIES = 

BINDIR = bin
NAME = httpServer
PRODUCT = $(BINDIR)/$(NAME)

all: $(PRODUCT)
	
$(PRODUCT): $(OBJECTS) $(BINDIR)
	cc $(LIBRARIES) -o $(PRODUCT) $(OBJECTS)

%.o: %.c
	cc -o $@ -c $<

clean:
	- rm *.o

spotless: clean
	- rm -r $(BINDIR)

$(BINDIR):
	mkdir bin
