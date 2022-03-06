#!/bin/bash
rm outFile inFile
touch outFile inFile
chmod 777 outFile inFile
(ls /dev | grep ttyUSB0 && echo 'Autopilot device found') || (echo 'Autopilot device not found; connect it to ttyUSB0...' && sleep 3 && exit)

echo 'SERIALPARSER STARTING...'
konsole --hold -e python serialParser.py &

echo 'FLIGHTGEAR STARTING...'
fgfs --aircraft=Rascal110-JSBSim --generic=file,in,2,inFile,LibelulaXProtocol --timeofday=morning --generic=file,out,2,outFile,LibelulaXProtocol


