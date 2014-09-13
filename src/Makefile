OBJECTFILES = cube.o game.o gui.o io_file.o mahjong.o movements.o
SOURCEFILES = cube.cc game.cc gui.cc io_file.cc mahjong.cc movements.cc
#DEBUG_FLAGS=-D DEBUG_MODE
CXXFLAGS=-Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS=-export-dynamic `pkg-config --libs gtk+-3.0`

Mahjong: $(OBJECTFILES)
	g++ -o Mahjong $(OBJECTFILES) $(LDFLAGS) $(CXXFLAGS)
	rm -f ./Mahjong_debug

-include dependencies

.PHONY: depend clean cleanall debug doc

depend:
	g++ -MM ./*.cc > dependencies
clean:
	rm -f ./*.o
cleanall:
	rm -f ./*.o
	rm -f ./*~
	rm -f ./Mahjong
	rm -f ./Mahjong_debug
debug:
	g++ -g -D DEBUG_MODE -c $(SOURCEFILES) $(LDFLAGS) $(CXXFLAGS)
	g++ -g -D DEBUG_MODE -o Mahjong_debug $(OBJECTFILES) $(LDFLAGS) $(CXXFLAGS)
	rm -f ./Mahjong
doc:
	doxygen Doxyfile
