# Debug mode
DEBUG=true

# INI file of Debug mode
DEBUG_FILE="\"./ini_files/test_old.ini\""

VARIABLES=-DDEBUG=$(DEBUG) -DDEBUG_FILE=$(DEBUG_FILE)

# C++ Files to compile
SOURCES=./sources/*.cpp

# Créer un fichier "iniparser" exécutable
iniparser:
	g++ $(VARIABLES) $(SOURCES) -o ./bin/iniparser

.PHONY: clean

clean: 
	rm -f ./bin/*

