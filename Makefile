# Project Name
TARGET = Kagouyar

# Sources
CPP_SOURCES = a_kagouyar.cpp

# Library Locations
LIBDAISY_DIR = ../../daisy/git/libDaisy
DAISYSP_DIR = ../../daisy/git/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

