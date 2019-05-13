FILENAME := pseudoOps
all:
	rgbasm -o ./$(FILENAME).o ./gbz80-pseudoOps.inc
	rgblink -o ./$(FILENAME).gb -n ./$(FILENAME).sym ./$(FILENAME).o
	rgbfix -v -p 0xFF ./$(FILENAME).gb