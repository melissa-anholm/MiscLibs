cmake_minimum_required (VERSION 2.6)
project (question_07 CXX)

#ROOT includes =========================
#the FindROOT.cmake script does all of the 
#magic for us to let us link to all of the 
#root libraries
#INCLUDE(${CMAKE_SOURCE_DIR}/FindROOT.cmake)  # This will look for findROOT.cmake right in the current directory.

INCLUDE($ENV{MISC_INCLUDE_DIR}/FindROOT.cmake)  # because findroot.cmake now lives in *this* directory.
INCLUDE_DIRECTORIES (SYSTEM ${ROOT_INCLUDE_DIR})  # ??????
INCLUDE_DIRECTORIES (${ROOT_INCLUDE_DIR})
INCLUDE_DIRECTORIES ($ENV{MISC_INCLUDE_DIR})  # Do I need this for anything?  maybe not right now.
#INCLUDE_DIRECTORIES (/usr/local/mysql/include)
LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
#LINK_DIRECTORIES (/usr/local/mysql/lib)  # manually put in something that looks like MYSQL_LIBRARIES .
LINK_DIRECTORIES (/usr/local/lib)  # for trinatdaq

#Compiler Flags=========================
SET(CMAKE_BUILD_TYPE ReLWithDebInfo)
#SET(CMAKE_CXX_FLAGS "-std=c++0x") 
#SET(CMAKE_CXX_FLAGS "-std=c++11") 
#SET(CMAKE_CXX_FLAGS "-std=gnu++11") 

#Adding the Libraries ==================
#add_library (bb1_strip /Users/spiffyzha/Packages/MiscLibs/BB1/bb1_strip.cpp)

#Linking the Executable ================
add_executable (madhadd MadHadder.cpp)  # inherits from loadgraph_Ecompare2.cpp

#link this library with everything that is needed
#to pop up a windows with the plots at the end
target_link_libraries (madhadd
             #          bb1_strip  
              ##         minuit   # trinat01 can't find minuit, even though it's right there...
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

