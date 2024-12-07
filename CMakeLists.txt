cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

#--- Project description ---
project(techTest
        LANGUAGES CXX
        DESCRIPTION "тестовое ПО"
)

if (NOT QT_CMAKE_DIR)
    # Если не определена, используем значение по умолчанию
    if (DEFINED ENV{QT_CMAKE_DIR})
        set(QT_CMAKE_DIR $ENV{QT_CMAKE_DIR})
    elseif(CMAKE_SYSROOT)
        # Попытка найти Qt в sysroot
        set(QT_CMAKE_DIR "${CMAKE_SYSROOT}/usr/lib/cmake")
    else()
        # Попытка найти Qt в домашней директории пользователя
        macro(qt_ver_locate retval curdir)
            file(GLOB lstDirs LIST_DIRECTORIES true ${curdir}/*)
            list(LENGTH lstDirs lstLen)
            if (lstLen EQUAL 0)
                message(FATAL_ERROR "Unable to locate qt version dir in specified folder ${curdir}")
                set(${retval} "")
            else()
                list(GET lstDirs 0 ${retval})
            endif()
        endmacro()

        qt_ver_locate(QT_VER_LOCATE "/home/$ENV{USER}/Qt")

        set(QT_CMAKE_DIR ${QT_VER_LOCATE}/gcc_64/lib/cmake)
    endif()
endif()

# Проверяем, существует ли директория QT_CMAKE_DIR
if (NOT EXISTS ${QT_CMAKE_DIR})
    message(FATAL_ERROR "Directory ${QT_CMAKE_DIR} with qt cmake files does not exist")
endif()

# Добавляем QT_CMAKE_DIR в CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE_DIR})

#----------------------------------
#--- Moc and resources compiler ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#--------------------
#--- C++ standart ---
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

#----------------
#--- Packages ---
find_package(Qt6 COMPONENTS
                            Core
                            Gui
                            Qml
                            Quick
                            Sql
                  REQUIRED)


#--------------------
#--- Source files ---
set(srcFiles
    main.cpp
    resources.qrc
)

#--------------------
#--- Build binary ---
add_executable(${PROJECT_NAME} ${srcFiles} ${resFiles}
    main_controller.h main_controller.cpp
    counts_model.h counts_model.cpp
    sql.cpp
    sql.h
)

#----------------------
#--- Link libraries ---
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::Sql
)


#--- Compile options ---
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -flto=auto
        -funroll-loops
        -std=c++${CMAKE_CXX_STANDARD}
)

#--------------------
#--- Link options ---
target_link_options(${PROJECT_NAME}
    PRIVATE
        -flto=auto
)
