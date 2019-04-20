#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle_jungk
# Author(s):                              Kimberly Jung
#--------------------------------------------------------

SET(SRC
  CommunicationAngle_jungk.cpp
  CommunicationAngle_jungk_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pCommunicationAngle_jungk ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle_jungk
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

