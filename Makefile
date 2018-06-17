EXEC = yall
CFLAGS   = -std=c99 -Wall -pedantic
LDFLAGS  = -std=c99 -Wall -pedantic

SRCD = src
INCD = include
OBJD = obj

OBJFILES = y_math.o mpc.o main.o
OBJS = $(OBJFILES:%.o=$(OBJD)/%.o)

default: init $(EXEC)

init:
	@mkdir -p $(OBJD)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJD)/%.o: $(SRCD)/%.c
	$(CC) -o $@ -c $< -I$(INCD) -I$(SRCD) $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -f $(OBJD)/*.o

mrproper: clean
	@rm -f $(EXEC)
