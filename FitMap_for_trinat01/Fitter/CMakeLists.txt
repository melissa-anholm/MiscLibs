cmake_minimum_required (VERSION 2.6)
project (question_07 CXX)

#ROOT includes =========================
#INCLUDE(${CMAKE_SOURCE_DIR}/FindROOT.cmake)  # This will look for findROOT.cmake right in the current directory.

INCLUDE($ENV{MISC_INCLUDE_DIR}/FindROOT.cmake)  # because findroot.cmake now lives in *this* directory.
INCLUDE_DIRECTORIES (SYSTEM ${ROOT_INCLUDE_DIR})  # ??????
INCLUDE_DIRECTORIES (${ROOT_INCLUDE_DIR})
INCLUDE_DIRECTORIES ($ENV{MISC_INCLUDE_DIR})  # Do I need this for anything?  maybe not right now.
INCLUDE_DIRECTORIES ($ENV{MISC_INCLUDE_DIR}/CartoLibs)  


LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
LINK_DIRECTORIES (/usr/local/lib)  # for trinatdaq
LINK_DIRECTORIES ($ENV{MISC_INCLUDE_DIR}/CartoLibs)  
LINK_DIRECTORIES ($ENV{MISC_INCLUDE_DIR})  

#Compiler Flags=========================
#SET(CMAKE_BUILD_TYPE ReLWithDebInfo)

#Adding the Libraries ==================
#add_library (bb1_strip /Users/spiffyzha/Packages/MiscLibs/BB1/bb1_strip.cpp)
#add_library (minuit_parameter /Users/spiffyzha/Packages/MiscLibs/MinuitWrapperWrapper_fitparam.cpp)
#add_library (minuit_wrapper /Users/spiffyzha/Packages/MiscLibs/MinuitWrapperWrapper3.cpp)
#add_library (some_functions /Users/spiffyzha/Packages/MiscLibs/SomeFunctions.cpp)
add_library (canvas_plots /Users/anholm/Packages/MiscLibs/CartoLibs/canvasplots.cpp)
add_library (map_utility  /Users/anholm/Packages/MiscLibs/CartoLibs/MapUtility.cpp)
add_library (fit_utility  /Users/anholm/Packages/MiscLibs/CartoLibs/FitUtility.cpp)
add_library (color_extras /Users/anholm/Packages/MiscLibs/ColorExtras.cpp)
add_library (hist_extras  /Users/anholm/Packages/MiscLibs/HistExtras.cpp)
add_library (graph_extras /Users/anholm/Packages/MiscLibs/GraphExtras.cpp)
# add_library (graph_extras GraphExtras.cpp)

#Linking the Executable ================
add_executable (doasym AsymCompare11.cpp)  # 

#link this library with everything that is needed
#to pop up a windows with the plots at the end
target_link_libraries (doasym
                       ncurses
                       color_extras
                       hist_extras
                       graph_extras
                       canvas_plots
                       map_utility
                       fit_utility
             #          bb1_strip  
             #          minuit_parameter
             #          some_functions
             #          minuit_wrapper
                       minuit
                       Gui
                       Core
                #       Cint #doesn't work with rootv6.  I can comment it out, but then does root still work?!
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

