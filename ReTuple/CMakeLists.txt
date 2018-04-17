cmake_minimum_required (VERSION 2.6)
project (Blah CXX)

#ROOT includes ===========================
#IF( $ENV{HOST} STREQUAL "trinat02.triumf.ca")
IF( $ENV{USER} STREQUAL "trinat")
	IF( $ENV{HOST} STREQUAL "trinat02.triumf.ca")
		INCLUDE(/home1/trinat/anholm/Packages/MiscLibs/FindROOT.cmake)   # trinat02.
	ELSE()
		INCLUDE(/home/trinat/anholm/MiscLibs/FindROOT.cmake)   # Trinatdaq.
	ENDIF()
ELSE() 
	INCLUDE($ENV{MISC_INCLUDE_DIR}/FindROOT.cmake)        # Home.
ENDIF() 

#blarg.
INCLUDE_DIRECTORIES (${ROOT_INCLUDE_DIR})
INCLUDE_DIRECTORIES (/home/trinat/anholm/MiscLibs)
INCLUDE($ENV{MISC_INCLUDE_DIR}/FindROOT.cmake)  # because findroot.cmake now lives in *this* directory.
INCLUDE_DIRECTORIES (SYSTEM ${ROOT_INCLUDE_DIR})  # ??????
INCLUDE_DIRECTORIES (${ROOT_INCLUDE_DIR})
INCLUDE_DIRECTORIES ($ENV{MISC_INCLUDE_DIR})  # Do I need this for anything?  maybe not right now.
LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
LINK_DIRECTORIES (/usr/local/lib)  # for trinatdaq



#Linking Things =========================
LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
#LINK_DIRECTORIES (/usr/local/mysql/lib)  # manually put in something that looks like MYSQL_LIBRARIES .
LINK_DIRECTORIES (/usr/local/lib)  # for trinatdaq


#Compiler Flags=========================
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -Wall -pthread -m64 -stdlib=libc++ -std=c++11")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -Wall -pthread -m64 -std=c++11")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -Wall -pthread -m64")
SET(CMAKE_BUILD_TYPE ReLWithDebInfo)

#Adding the Libraries ==================
IF( $ENV{USER} STREQUAL "trinat")  #
	IF($ENV{HOST} STREQUAL "trinat02.triumf.ca")  
		add_library (bb1_strip /home1/trinat/anholm/Packages/MiscLibs/BB1/bb1_strip.cpp)
		#ELSEIF( $ENV{USER} STREQUAL "trinat")  # on trinatdaq.
	ELSE()  # on trinatdaq.
		add_library (bb1_strip /home/trinat/anholm/MiscLibs/BB1/bb1_strip.cpp)
	ENDIF()
ELSE()
	IF($ENV{LOCATION} STREQUAL "trinat01")
	add_library (bb1_strip /home/anholm/MiscLibs/BB1/bb1_strip.cpp)  # 
	ELSE()
	add_library (bb1_strip $ENV{MISC_INCLUDE_DIR}/BB1/bb1_strip.cpp)  # Home.
	ENDIF()
ENDIF()

#Linking the Executable ================
#create exectuable
add_executable (retuple TupleMyTuple.cpp)
#add_executable (retuple TupleMyTuple_kludge.cpp)
#add_executable (testtuple TestTuple.cpp)
#add_executable (calibrationator TheCalibrationator.cpp)

#link this library with everything that is needed
#to pop up a windows with the plots at the end
target_link_libraries (retuple
                       bb1_strip
                       Gui
                       Core
                       Cint #doesn't work with rootv6.  I can comment it out, but then does root still work?!
                       RIO
                       Net
                       Hist
                       Graf
                       Graf3d
                       Gpad
                       Tree
                       Rint
                       Postscript
                       Matrix
                       Physics
                       MathCore
                       Thread
                       pthread
                       m
                       dl
                       )   ## does this even work?!

