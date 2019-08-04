CC=gcc
DEPS = All_Data/data.h Battery_processing/BatteryOperation.h
OBJ = All_Data/data.o Battery_processing/Battery_Operation.o application.o
CFLAGS = -g 

%.o:%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

final : $(OBJ)
	gcc -o $@ $^ 	
	
clean : 
	rm -rfv All_Data/*o Battery_processing/*o /*o
