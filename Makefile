SHELL := /bin/bash

CC ?= gcc
LD ?= gcc

# compile options
ap ?= float

CPPFLAGS = $(extra)

ifeq "$(ap)" "float"
	CPPFLAGS += -D_FLOAT_PRECISION_
endif

ifeq "$(config)" "release"
	CPPFLAGS += -O3 -fno-expensive-optimizations -DNDEBUG -D_GNU_SOURCE
else
	CPPFLAGS += -g -D_GNU_SOURCE
endif

CPPFLAGS += -Wno-deprecated -Winline -Wall
CPPFLAGS += -IwiringPi
CFLAGS := $(CPPFLAGS)
CPPFLAGS += -std=c++11

LIBS += -lstdc++ -lm -lz -lpthread

VPATH := ./wiringPi
.DEFAULT_GOAL := led++

OBJECTS = main.o
# apa102
OBJECTS += apa102.o
# wiringPI / SPI
OBJECTS += wiringPi.o wiringPiSPI.o softPwm.o softTone.o piHiPri.o

all: led++

led++: $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

show:
	@echo "CC       = $(CC)"
	@echo "OBJECTS  = $(OBJECTS)"
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "CPPFLAGS = $(CPPFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "EXTRA    = $(extra)"

clean:
	rm -f *.o led++ *~
