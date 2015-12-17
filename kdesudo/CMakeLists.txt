configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#  set(kt4_SRC app.cpp mainwindow.cpp core.cpp view.cpp viewmodel.cpp main.cpp)
set(KDESUDO_SRC
	main.cpp
	kdesudo.cpp
	kcookie.cpp
)

kde4_add_executable(kdesudo ${KDESUDO_SRC})

target_link_libraries(kdesudo ${KDE4_KDEUI_LIBS})
install(TARGETS kdesudo DESTINATION ${BIN_INSTALL_DIR})
