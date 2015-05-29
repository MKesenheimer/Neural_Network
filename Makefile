########################################################################
#                          -*- Makefile -*-                            #
########################################################################

COMPILER = g++ -std=c++11

########################################################################
## Flags
FLAGS   = -g
LDFLAGS =
PREPRO  =
##verbose level 1
#DEBUG   = -D DEBUGV1
##verbose level 2
#DEBUG  += -D DEBUGV2
##verbose level 3
#DEBUG  += -D DEBUGV3
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
vpath %.a $(WORKINGDIR)/build
UINCLUDE = $(WORKINGDIR)

########################################################################
## Includes
CXX  = $(COMPILER) $(FLAGS) $(OPT) $(WARN) $(DEBUG) $(PREPRO) -I$(UINCLUDE)
INCLUDE = $(wildcard *.h $(UINCLUDE)/*.h)

%.a: %.cpp $(INCLUDE)
	$(CXX) -c -o build/$@ $<

# Libraries
LIB =

# Frameworks
FRM = 

########################################################################
## Linker files

### USER Files ###
USER = Main.a Neuron.a Synapse.a Brain.a Trainer.a Helper.a


########################################################################
## Rules
## type make -j4 [rule] to speed up the compilation

BUILD = $(USER)

Brain: $(BUILD)
	  $(CXX) $(patsubst %,build/%,$(BUILD)) $(LDFLAGS) $(LIB) $(FRM) -o $@

clean:
	rm -f build/*.a Brain

do:
	make && ./Brain

########################################################################
#                       -*- End of Makefile -*-                        #
########################################################################
