########################################################################
#                          -*- Makefile -*-                            #
########################################################################

COMPILER = g++

########################################################################
## Flags
FLAGS   = -g
LDFLAGS =
PREPRO  = 
#DEBUG   = -D DEBUG
OPT     = -O2
WARN    =

### generate directory obj, if not yet existing
$(shell mkdir -p build)

########################################################################
## Paths

WORKINGDIR = $(shell pwd)
PARENTDIR  = $(WORKINGDIR)/..

########################################################################
## search for the files and set paths

vpath %.cpp $(WORKINGDIR)
vpath %.o $(WORKINGDIR)/build
UINCLUDE = $(WORKINGDIR)

########################################################################
## Includes
CXX  = $(COMPILER) $(FLAGS) $(OPT) $(WARN) $(DEBUG) $(PREPRO) -I$(UINCLUDE)
INCLUDE = $(wildcard *.h $(UINCLUDE)/*.h)

%.o: %.cpp $(INCLUDE)
	$(CXX) -c -o build/$@ $<

# Libraries
LIB =

# Frameworks
FRM = 

########################################################################
## Linker files

### USER Files ###
USER = Main.o Neuron.o Brain.o helper.o


########################################################################
## Rules
## type make -j4 [rule] to speed up the compilation

BUILD = $(USER)

Brain: $(BUILD)
	  $(CXX) $(patsubst %,build/%,$(BUILD)) $(LDFLAGS) $(LIB) $(FRM) -o $@

clean:
	rm -f build/*.o Brain

do:
	make && ./Brain

########################################################################
#                       -*- End of Makefile -*-                        #
########################################################################
