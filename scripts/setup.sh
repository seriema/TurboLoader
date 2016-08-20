#!/bin/bash

# Installs project dependencies.

echo "** Set up project **"

# --- RetroPie -----------------------------------------------------------------
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	echo "* RetroPie setup *"
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libfreeimage-dev libfreetype6-dev lua5.1-dev libboost-all-dev

# --- OSX ----------------------------------------------------------------------
elif [[ "$OSTYPE" == "darwin"* ]]; then
	echo "* Mac OSX setup *"
	brew update
	brew unlink boost # boost is usually installed
	brew install sdl2 freeimage freetype lua51 boost

fi
