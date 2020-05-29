LDLIBS += -lmosquitto -lm -l bcm2835

TARGET = main

INCLUDES = -I./cJSON 

OBJECTS = main.o \
	./cJSON/cJSON.o \

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -Wall -std=gnu99 -g -c $< -o $@


all: clean build

build: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET) $(LDLIBS)
	#cp $(TARGET) ../../swupdate/images/usr/bin/

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	#-rm -f ../../swupdate/images/usr/bin/$(TARGET) 