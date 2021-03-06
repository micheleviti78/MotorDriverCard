project(QtMotorConfigCalculator)

FIND_PACKAGE(Qt4 REQUIRED)

# current binary dir for the generated headers from the ui and the moc
# Treat them as system directories because they contain generated code which cannot directly be controlled by us.
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}  ${PROJECT_NAME}_SOURCES)

SET(${PROJECT_NAME}_QOBJECT_HEADERS MotorConfigWidget.h QtMotorConfigCalculator.h HexSpinBox.h)
#SET(${PROJECT_NAME}_HEADERS ${PROJECT_NAME}_QOBJECT_HEADERS PlotWindow.h)
SET(${PROJECT_NAME}_FORMS MotorConfigWidget.ui)
SET(${PROJECT_NAME}_RESOURCES ${PROJECT_NAME}.qrc)

QT4_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOC ${${PROJECT_NAME}_QOBJECT_HEADERS})
QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
message("${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS_HEADERS} ")
QT4_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

#INCLUDE(${QT_USE_FILE})
# Do not use INCLUDE(${QT_USE_FILE}). What this does is run
# /usr/share/cmake-2.8/Modules/UseQt4.cmake which sets the variables for you
# (including QT_LIBRARIES)
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(SYSTEM ${QT_INCLUDES})

ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} 
  ${${PROJECT_NAME}_HEADERS_MOC}
  ${${PROJECT_NAME}_FORMS_HEADERS} 
  ${${PROJECT_NAME}_RESOURCES_RCC})
# Link the required Qt libraries componentwise
TARGET_LINK_LIBRARIES(${PROJECT_NAME} mtca4u-MotorDriverCard ConfigCalculator 
                      ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY}  )

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

