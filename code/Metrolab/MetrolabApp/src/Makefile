TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = Metrolab
# Metrolab.dbd will be created and installed
DBD += Metrolab.dbd

# Metrolab.dbd will be made up from these files:
Metrolab_DBD += base.dbd

# Include dbd files from all support applications:
Metrolab_DBD += devMetrolab.dbd
Metrolab_DBD += drvAsynSerialPort.dbd

# Add all the support libraries needed by this IOC
Metrolab_LIBS += devMetrolab
Metrolab_LIBS += asyn

# Metrolab_registerRecordDeviceDriver.cpp derives from Metrolab.dbd
Metrolab_SRCS += Metrolab_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
Metrolab_SRCS_DEFAULT += MetrolabMain.cpp
Metrolab_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#Metrolab_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
Metrolab_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

