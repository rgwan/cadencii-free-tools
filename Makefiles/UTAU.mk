#Simple Makefile to Convert UTAU Soundbank to RUCE Soundbank

RUDBList = $(patsubst %.wav,%.rudb,$(wildcard *.wav)) 
.PHONY : SoundBank 
SoundBank:$(RUDBList) oto.ini
	@echo "Soundbank Built successfully" 
%.rudb:%.wav
	@echo " GENRUDB $^"
	@genrudb $^
oto.ini:$(RUDBList)
	@echo " GEN oto.ini"
	@rm -f oto.ini
	@for name in `echo $(RUDBList)`; \
	do \
		proberudb -u "$$name" >> oto.ini; \
	done 
.PHONY : clean
clean:
	@echo " CLEAN *.rudb"
	@rm -f *.rudb
	@echo " CLEAN oto.ini"
	@rm -f oto.ini
