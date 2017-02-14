CC = g++
CFLAGS = -Wall -g -O3 -std=c++0x 
#-DDEBUG
INCLUDES = # -I/home/newhall/include  -I../include
LFLAGS = # -L/home/newhall/lib  -L../lib
LIBS = -lrt

SRC1 = gen_trace.cc random_pattern.cc\
       row_access_trace.cc row_write_trace.cc\
       row_part_access_trace.cc row_part_write_trace.cc\
       col_part_access_trace.cc col_part_write_trace.cc\
       hybrid_access_trace.cc DRAM_access_trace.cc addr_util.cc
OBJ1 = $(SRC1:.cc=.o)
MAIN1 = gen_trace.exe
DEP1 := $(OBJ1:.o=.d)

.PHONY:	clean

all: $(MAIN1)

$(MAIN1): $(OBJ1)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN1) $(OBJ1) $(LFLAGS) $(LIBS)

%.o: %.cc
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	$(CC) -MM -MF $(patsubst %.o,%.d,$@) $(CFLAGS) $<

clean:
	$(RM) -f *.o *.d *.trace *~ $(MAIN1) $(MAIN2) $(MAIN3) $(MAIN4)

-include $(DEP1)
