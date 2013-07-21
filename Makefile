CPP = g++
CFLAGS = -Iinclude -O2
LFLAGS = lib/libfetion_32.a -lcurl -pthread 

BIN = sendsms
SRCS = $(BIN).c
OBJS = $(BIN).o 

$(BIN): $(OBJS)
	$(CPP) $^ $(LFLAGS) -o $@ 

$(OBJS) : %.o : %.c
	$(CPP) $(CFLAGS) -c $<

install: $(BIN)
	install $< /usr/bin

clean:
	@rm -rf *.o

distclean: clean
	@rm -f $(BIN)
