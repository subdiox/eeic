CC = gcc
CFLAGS = -Wall -g
#CFLAGS = -O2 -DNDEBUG
LDLIBS = -lm
OBJS = main.o func2.o optimize.o
TARGET = optimizer

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

.PHONY: tmpclean clean
tmpclean:
	rm -f *~
clean: tmpclean
	rm -f $(OBJS) $(TARGET)
