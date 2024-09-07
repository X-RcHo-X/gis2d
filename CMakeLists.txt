# Указываем минимальную версию CMake
cmake_minimum_required (VERSION 3.7.2 FATAL_ERROR)

# Устанавливаем имя проекта. Аргумент LANGUAGES показывает, что приложение написано на C++
project(Gis2D LANGUAGES CXX)

# Настройки проекта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ищем Qt5 и импортируем модули Core, Widgets, Gui
# Аргумент REQUIRED остановит сборку если не сможет найти хотя бы 1 из указанных модулей
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# Создадим переменную ROOT_SRC_PATH которая укажет путь до CMakeLists.txt
set(ROOT_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Исполняемые файлы
set(SRC_CPP
	"${ROOT_SRC_PATH}/src/source/main.cpp"
	
	"${ROOT_SRC_PATH}/src/source/base_window.cpp"
	"${ROOT_SRC_PATH}/src/source/base_window.h"
	"${ROOT_SRC_PATH}/src/source/gis2d.cpp"
	"${ROOT_SRC_PATH}/src/source/gis2d.h"
	"${ROOT_SRC_PATH}/src/source/layer_manager.cpp"
	"${ROOT_SRC_PATH}/src/source/layer_manager.h"
	"${ROOT_SRC_PATH}/src/source/planimetr_and_ruler_layer.cpp"
	"${ROOT_SRC_PATH}/src/source/planimetr_and_ruler_layer.h"
	"${ROOT_SRC_PATH}/src/source/point_layer.cpp"
	"${ROOT_SRC_PATH}/src/source/point_layer.h"
	"${ROOT_SRC_PATH}/src/source/tile_layer.cpp"
	"${ROOT_SRC_PATH}/src/source/tile_layer.h"
	
	"${ROOT_SRC_PATH}/src/source/base_layer.h"
	"${ROOT_SRC_PATH}/src/source/base_styler.h"	
	"${ROOT_SRC_PATH}/src/source/base_source_functions.h"
	"${ROOT_SRC_PATH}/src/source/constants.h"
	"${ROOT_SRC_PATH}/src/source/user_data_types.h"
)
	
# Формы
set(SRC_UI
	"${ROOT_SRC_PATH}/src/source/ui/base_window.ui"
)

# Файл ресурсов для стилей
set(STYLE_RESOURCE
	"${ROOT_SRC_PATH}/src/resource/qss/resource.qrc"
)

# Компилируем ресурсы в исходный код и помещаем их в переменную RESOURCES
qt5_add_resources(RESOURCES ${STYLE_RESOURCE})

# Генерим заголовки UI
qt5_wrap_ui(UI_HEADERS ${SRC_UI})

# Полный перечень файлов
set(COMPLETE_SOURCE_LIST ${SRC_CPP} ${SRC_UI} ${UI_HEADERS} ${RESOURCES})

# Создаём исполняемый файл из тех файлов, которые указаны в переменной COMPLETE_SOURCE_LIST
add_executable(${PROJECT_NAME} ${COMPLETE_SOURCE_LIST})

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    EXPORT_NAME ${PROJECT_NAME}
    OUTPUT_NAME ${PROJECT_NAME}
    
    DEBUG_POSTFIX "-debug"
)

# Сообщает CMake, что исполняемый файл  использует Qt5::Widgets Qt5::Gui Qt5::Core, импортированную find_package()
target_link_libraries (${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Gui Qt5::Core)