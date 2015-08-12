TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Build devMetrolab as a library for an IOC:
LIBRARY_IOC += devMetrolab

# Library Source files
devMetrolab_SRCS += devMetrolab.c

# Link with the asyn and base libraries
devMetrolab_LIBS += asyn
devMetrolab_LIBS += $(EPICS_BASE_IOC_LIBS)

# Install .dbd and .db files
DBD += devMetrolab.dbd
DB_INSTALLS += ../devMetrolab.db

#=======================================
include $(TOP)/configure/RULES
