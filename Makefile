# Project Name
TARGET = Kagouyar

# Sources
CPP_SOURCES = a_kagouyar.cpp

# Library Locations
LIBDAISY_DIR = ../git_daisy/libDaisy
DAISYSP_DIR = ../git_daisy/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

