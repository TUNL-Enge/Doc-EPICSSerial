#!../../bin/linux-x86_64/Metrolab

## You may have to change Metrolab to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/Metrolab.dbd"
Metrolab_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/devMetrolab.db","P=Metrolab:,R=,L=0,A=0")
#dbLoadRecords("db/devMetrolab.db","user=longland")

## Serial port
drvAsynSerialPortConfigure("L0","/dev/ttyUSB0",0,0,0) 
asynSetOption("L0", -1, "baud", "19200") 
asynSetOption("L0", -1, "bits", "8") 
asynSetOption("L0", -1, "parity", "none") 
asynSetOption("L0", -1, "stop", "1") 
asynSetOption("L0", -1, "clocal", "Y") 
asynSetOption("L0", -1, "crtscts", "N")

## Debugging
#asynSetTraceMask("L0",-1,0x9) 
#asynSetTraceIOMask("L0",-1,0x2)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=longland"
