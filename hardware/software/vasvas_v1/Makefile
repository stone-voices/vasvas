# Project Name
TARGET = nb4

CPP_STANDARD = -std=c++20

#APP_TYPE = BOOT_SRAM

USE_DAISYSP_LGPL = 1

# Compiler options
#OPT = -O1
OPT=-Os
CFLAGS += -Os  
CXXFLAGS += -Os  

# Sources
CPP_SOURCES = main.cpp
CPP_SOURCES += tools.cpp 
CPP_SOURCES += fastsin.cpp
CPP_SOURCES += delaybase.cpp
CPP_SOURCES += allpass.cpp
CPP_SOURCES += delayline.cpp
CPP_SOURCES += reverb.cpp

# Library Locations
LIBDAISY_DIR = ../../DaisyExamples/libDaisy
DAISYSP_DIR = ../../DaisyExamples/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

C_INCLUDES += -I../../q/q_lib/include
C_INCLUDES += -I../../gcem/include
C_INCLUDES += -I../../infra/include





