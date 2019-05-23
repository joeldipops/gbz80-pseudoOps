FILENAME := pseudoOps
all:
	rgbasm -o ./$(FILENAME).o ./test/test.asm
	rgblink -o ./$(FILENAME).gb -n ./$(FILENAME).sym ./$(FILENAME).o
	rgbfix -v -p 0xFF ./$(FILENAME).gb