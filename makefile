# change application name here (executable output name)
TARGET=WebServer
# compiler
CC=g++
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall -Wextra -Werror
#src directory
SRCDIR=src
#object directoy
CPPVERSION=-std=c++17
OBJDIR=obj

#target directory

CCFLAGS=$(DEBUG) ${CPPVERSION} $(OPT) $(WARN) -pipe

LIBCCFLAGS=$(DEBUG) ${LIBFLAGS} ${CPPVERSION} $(OPT) $(WARN)
# linker
LD=g++
LDFLAGS=-lstdc++

OBJS=$(OBJDIR)/main.o \
$(OBJDIR)/Request.o \
$(OBJDIR)/directory.o


all: $(OBJS)
	$(LD) -o build/$(TARGET) $(OBJS)  $(LDFLAGS)

$(OBJDIR)/main.o:
	$(CC) -c $(CCFLAGS) $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/Request.o:
	$(CC) -c $(CCFLAGS) $(SRCDIR)/Request.cpp -o $(OBJDIR)/Request.o

$(OBJDIR)/directory.o:
	$(CC) -c $(CCFLAGS) $(SRCDIR)/directory.cpp -o $(OBJDIR)/directory.o
run: 
	./build/WebServer 9999
	
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)