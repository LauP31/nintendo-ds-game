# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= homebrew_game_one
GAME_TITLE	:= Infinite runner game
GAME_SUBTITLE	:= 
GAME_AUTHOR ?=

# Source code paths

SOURCEDIRS := source
AUDIODIRS	:= audio
GFXDIRS		:= graphics

LIBS		:= -lnds9 -lmm9
LIBDIRS		:= $(BLOCKSDS)/libs/maxmod

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
