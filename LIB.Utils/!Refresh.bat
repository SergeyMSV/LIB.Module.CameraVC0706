del /Q *.cpp
del /Q *.h

xcopy /Y %LIB_UTILS%\utilsBase.*
xcopy /Y %LIB_UTILS%\utilsChrono.*
xcopy /Y %LIB_UTILS%\utilsLog.*
xcopy /Y %LIB_UTILS%\utilsPacket.*
xcopy /Y %LIB_UTILS%\utilsPacketCameraVC0706.*
xcopy /Y %LIB_UTILS%\utilsPath.*
xcopy /Y %LIB_UTILS%\utilsSerialPort.*
xcopy /Y %LIB_UTILS%\utilsShell.*

pause
