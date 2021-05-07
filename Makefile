EXEC = $(shell basename $(CURDIR))
SYMBOLS =
INC =
CFLAGS = -O0 $(SYMBOLS) $(INC)

all: $(EXEC)

$(EXEC): $(shell basename $(CURDIR)).c
	$(CC) $+ $(CFLAGS) -o $(EXEC)

clean:
	rm -rf $(EXEC)
