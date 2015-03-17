#
# Makefile
#

BINDIR = bin
TESTDIR = tests
NAME = httpServer
PRODUCT = $(BINDIR)/$(NAME)

MAINSOURCE = httpServer.c
MAINOBJECT = $(patsubst %.c,%.o,$(MAINSOURCE))

CSOURCE = config.c
OBJECTS = $(patsubst %.c,%.o,$(CSOURCE))
LIBRARIES = -lyaml

TESTSOURCE = $(wildcard tests/*)
TESTOBJECTS = $(patsubst %.c,%.o,$(TESTSOURCE))
TESTS = $(patsubst %.c,%,$(TESTSOURCE))
RUNTESTS = $(patsubst %.c,%.runtest,$(TESTSOURCE))

all: $(PRODUCT)
	
$(PRODUCT): $(MAINOBJECT) $(OBJECTS) $(BINDIR)
	cc $(LIBRARIES) -o $(PRODUCT) $(MAINOBJECT) $(OBJECTS)

%.o: %.c
	cc -o $@ -c $<

clean:
	- rm *.o
	- rm $(TESTDIR)/*.o

spotless: clean
	- rm -r $(BINDIR)

$(BINDIR):
	mkdir bin

test: $(RUNTESTS)

%.runtest: %
	./$<

%Test: %Test.o $(OBJECTS)
	cc $(LIBRARIES) -o $@ $< $(OBJECTS)


