#-----------------------------------------------------------------------------------------------------
# Copyright (C) 2017-2018 Jilles Steeve Dibangoye
#
#
# Makefile
# Project -- Robot Cleaner 1.0
#
# AUTHORS
#
# Jilles S. Dibangoye  -- contacts <jilles-steeve.dibangoye@insa-lyon.fr, jilles.dibangoye@inria.fr>
#-----------------------------------------------------------------------------------------------------

# Determine the platform
UNAME_S       :=	$(shell uname -s)

#-----------------------------------------------------------------------------------------------------
ifeq ($(UNAME_S), Darwin)
	SYSTEM			:=	x86-64_osx
else
	SYSTEM			:=	x86-64_linux
endif

LIBFORMAT			:=	static_pic

#-----------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------
ifeq ($(UNAME_S), Darwin)
BOOSTDIR			:=	/usr/local/Cellar/boost/1.64.0_1
else

endif

SRCDIR				:=	src
BINDIR				:=	build
OBJDIR				:=	obj


#-----------------------------------------------------------------------------------------------------
# Target executable
#-----------------------------------------------------------------------------------------------------
TARGET				:=	$(BINDIR)/robot_cleaner

#-----------------------------------------------------------------------------------------------------
# Compiler selection
#-----------------------------------------------------------------------------------------------------

ifeq ($(UNAME_S), Darwin)
	CXX	:=	g++
else
	CXX	:=	g++
endif

#-----------------------------------------------------------------------------------------------------
# Link options and libraries
#-----------------------------------------------------------------------------------------------------
BOOSTLIBDIR		:=	$(BOOSTDIR)/lib

CXXLNDIRS		  := -L$(BOOSTLIBDIR)
CXXLNFLAGS		:= -lboost_iostreams -lboost_filesystem -lboost_system

OPENMPFLAGS		:= -fopenmp=libiomp5

CPLEXFLAGS		:= -isystem$(CPLEXDIR)/include -isystem$(CONCERTDIR)/include
CPLEXLNFLAGS  := -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -m64 -lpthread

ifeq ($(UNAME_S), Darwin)
	BOOSTINCDIR	:= $(BOOSTDIR)/include/boost
else
	BOOSTINCDIR	:= $(BOOSTDIR)/include/boost
endif

CXXFLAGS			:= $(CXXOPT) -I$(BOOSTINCDIR)

ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -O0 -W -Wall -pedantic -std=c++1y -DIL_STD
else
	CXXFLAGS += -O0 -W -Wall -pedantic -std=c++1y -DIL_STD
endif

#-----------------------------------------------------------------------------------------------------
# make all	:	to compile the sources.
# make execute	:	to execute the examples.
#-----------------------------------------------------------------------------------------------------
EXT						:=	cpp

#-----------------------------------------------------------------------------------------------------
# Code lists
#-----------------------------------------------------------------------------------------------------
SRC						:=	$(shell find $(SRCDIR) -type f -name *.$(EXT))
OBJ						:=	$(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.$(EXT)=.o))
OBJ_NO_MAIN		:=	$(filter-out $(OBJDIR)/main.o,$(OBJ))


#-----------------------------------------------------------------------------------------------------
# Folder lists
#-----------------------------------------------------------------------------------------------------
INCDIRS				:=	$(shell find include/* -name '*.hpp' -exec dirname {} \; | sort | uniq)
INCLIST				:=	$(patsubst include/%, -isystem include/%, $(INCDIRS))
BINLIST				:=	$(patsubst include/%, $(BINDIR)/%, $(INCDIRS))
INC						:=	-isystem include $(INCLIST)

#------------------------------------------------------------------------------------------------------
# Default target
default:	all

# Target all builds the executable
all:		$(TARGET)

# Current size of the projet
count:
			@ find . "(" -name "Makefile" -or -name "*.hpp" -or -name "*.cpp" ")" -exec wc -l {} +

debug: CXXFLAGS += -DDEBUG -pg -g
debug: $(TARGET)

# The constant DEBUG is set at yes
ifeq ($(DEBUG),yes)
		CXXFLAGS += -pg -g
endif

#-----------------------------------------------------------------------------------------------------
.PHONY:		help clean mrproper

# Display the help message
help:
		@ echo 'Makefile for the project robot cleaner '
		@ echo ''
		@ echo 'Usage: '
		@ echo 'make		Building project executable'
		@ echo 'make all	Building project executable'
		@ echo 'make clean	Clean objects directory'
		@ echo 'make mrproper	Delete objects and binary directory'
		@ echo 'make help	Show this help message'
		@ echo ''

clean:
		@ echo '\033[1m\033[32m ######################## Cleaning...\033[0m'
		@ echo 'rm -rf $(OBJ) *~'
		@ rm -rf $(OBJ) *~

mrproper:
		@ echo 'Deleting ...'
		@ echo 'rm -rf $(OBJDIR) $(BINDIR) $(TARGET) *.lp *.txt *~'
		@ rm -rf $(OBJDIR) $(BINDIR) $(TARGET) *.lp *.txt *~

#-----------------------------------------------------------------------------------------------------
# Sources
#-----------------------------------------------------------------------------------------------------
$(TARGET):	$(OBJ)
		@ echo 'Linking ...'
		@ echo 'Linking $(TARGET)'
		@ mkdir -p $(BINDIR)
		@ echo '$(CXX) -o $@ $^ $(CXXLNDIRS) $(CXXLNFLAGS) $(CXXFLAGS) '; $(CXX) -o $@ $^ $(CXXLNDIRS) $(CXXLNFLAGS) $(CXXFLAGS)

$(OBJDIR)/%.o:	$(SRCDIR)/%.$(EXT)
		@ echo 'Compiling $< ...'
		@ mkdir -p $(OBJDIR)
		@ echo '$(CXX) -c -o $@ $< $(INC) $(CXXFLAGS)'; $(CXX) -c -o $@ $< $(INC) $(CXXFLAGS)

$(BINDIR)/%.exe: $(OBJ_NO_MAIN) $(OBJDIR)/%.o
		@ echo 'Linking ...'
		@ echo 'Linking $@'
		@ mkdir -p $(BINDIR)
		@ echo '$(CXX) -o $@ $^ $(CXXLNDIRS) $(CXXLNFLAGS) $(CXXFLAGS)'; $(CXX) -o $@ $^ $(CXXLNDIRS) $(CXXLNFLAGS) $(CXXFLAGS)

#-----------------------------------------------------------------------------------------------------

# Local Variables:
# mode: makefile
# End:
