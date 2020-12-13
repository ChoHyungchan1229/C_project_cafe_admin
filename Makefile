CC = gcc
TARGET = cafe.out
OBJS = cafe_main.o

CFLAGS = -Wall
LDFLAGS = -lc

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CLFLAGS) -c -o $@ $<

clean : 
	rm -f $(OBJS) $(TARGET)
