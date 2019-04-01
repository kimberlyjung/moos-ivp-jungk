#--------------------------------------------------------
# The CMakeLists.txt for:                       pMessageTimer
# Author(s):                              Kimberly Jung
#--------------------------------------------------------

SET(SRC
  MessageTimer.cpp
  MessageTimer_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pMessageTimer ${SRC})

TARGET_LINK_LIBRARIES(pMessageTimer
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

