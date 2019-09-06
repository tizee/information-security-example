CC=g++
IDIR=.
ODIR=output
INCLUDE=-I$(IDIR)
STD=c++11


all:
	mkdir -p $(ODIR) && $(CC) -Wall -std=$(STD) $(INCLUDE) main.cpp des_fileencryption.cpp cbc.cpp cfb.cpp des_.cpp -o $(ODIR)/des

.PHONEY: clean

clean:
	rm -f $(ODIR)/*

