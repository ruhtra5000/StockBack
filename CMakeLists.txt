cmake_minimum_required(VERSION 3.5)

project(StockBack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)
find_package(Qt6 REQUIRED COMPONENTS Gui)

set(PROJECT_SOURCES
        main.cpp
        formprincipal.cpp formprincipal.h formprincipal.ui
        formlogin.h formlogin.cpp formlogin.ui
        formgestaoestoque.h formgestaoestoque.cpp formgestaoestoque.ui
        formeditarproduto.h formeditarproduto.cpp formeditarproduto.ui
        formgestaofunc.h formgestaofunc.cpp formgestaofunc.ui
        formeditarfunc.h formeditarfunc.cpp formeditarfunc.ui
        formgestaovendas.h formgestaovendas.cpp formgestaovendas.ui
        formnovavenda.h formnovavenda.cpp formnovavenda.ui
        formpesquisarproduto.h formpesquisarproduto.cpp formpesquisarproduto.ui
        formeditarprodutovenda.h formeditarprodutovenda.cpp formeditarprodutovenda.ui
        formgestaofornecedores.h formgestaofornecedores.cpp formgestaofornecedores.ui
        formeditarfornecedor.h formeditarfornecedor.cpp formeditarfornecedor.ui
        formnovaaquisicao.h formnovaaquisicao.cpp formnovaaquisicao.ui
        formpesquisaraquisicao.h formpesquisaraquisicao.cpp formpesquisaraquisicao.ui
        Conexao.h
        Recursos.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StockBack
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET StockBack APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(StockBack SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(StockBack
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(StockBack PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(StockBack PRIVATE Qt6::Sql)
target_link_libraries(StockBack PRIVATE Qt6::PrintSupport)
target_link_libraries(StockBack PRIVATE Qt6::Gui)

set_target_properties(StockBack PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS StockBack
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StockBack)
endif()
