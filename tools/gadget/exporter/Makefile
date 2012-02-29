
TARGET = gscr

CC = gcc -std=c99
INCLUDE =  -I. -I./include
WARN = -Wall
COMPILERFLAGS = -c -g $(INCLUDE) $(WARN)
CFLAGS += $(COMPILERFLAGS)

LIBPATH += 
LIBRARIES += 
OBJDIR = .
HEADERS = $(shell ls *.h) 
SOURCES = $(shell ls ./*.c)
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(SOURCES))
	
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBPATH) $(LIBRARIES)
			
clean:
	rm -f $(TARGET) $(OBJECTS)
		
.DEFAULT:
	@echo The target \"$@\" does not exist in this Makefile.
