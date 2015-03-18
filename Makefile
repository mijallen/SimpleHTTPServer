#
# Makefile
#

BINDIR = bin
TESTDIR = tests
LIBDIR = lib
NAME = httpServer
PRODUCT = $(BINDIR)/$(NAME)

MAINSOURCE = httpServer.c
MAINOBJECT = $(patsubst %.c,%.o,$(MAINSOURCE))

CSOURCE = config.c
OBJECTS = $(patsubst %.c,%.o,$(CSOURCE))
STATICLIB = $(LIBDIR)/libyaml.a
LIB = 

TESTSOURCE = $(wildcard tests/*)
TESTOBJECTS = $(patsubst %.c,%.o,$(TESTSOURCE))
TESTS = $(patsubst %.c,%,$(TESTSOURCE))
RUNTESTS = $(patsubst %.c,%.runtest,$(TESTSOURCE))

all: $(PRODUCT)

# Building
$(PRODUCT): $(MAINOBJECT) $(OBJECTS) $(BINDIR) $(STATICLIB)
	cc $(LIB) -o $(PRODUCT) $(MAINOBJECT) $(OBJECTS) $(STATICLIB)

%.o: %.c
	cc -o $@ -c $<

# Libraries
$(LIBDIR)/libyaml.a:
	cd $(LIBDIR)/libyaml; ./bootstrap; ./configure; make;
	cp $(LIBDIR)/libyaml/src/.libs/libyaml.a $(LIBDIR);

# Cleaning
clean:
	- rm *.o
	- rm $(TESTDIR)/*.o

spotless: clean
	- rm -r $(BINDIR)

# Directories
$(BINDIR):
	mkdir bin

# Testing
test: $(RUNTESTS)

%.runtest: %
	./$<

%Test: %Test.o $(OBJECTS) $(STATICLIB)
	cc $(LIB) -o $@ $< $(OBJECTS) $(STATICLIB)


