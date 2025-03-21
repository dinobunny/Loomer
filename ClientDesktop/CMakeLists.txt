# Client/CMakeLists.txt

project(ClientDesktop LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

file(GLOB_RECURSE Client_SRC CONFIGURE_DEPENDS "*.h" "*.hpp" "*.cpp" "*.ui")

#добавив бо в Clion не може знайти цей файл то му що moc не може згенеровати файл
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


qt_add_executable(ClientDesktop
    ${Client_SRC}
)

target_link_libraries(ClientDesktop
    PRIVATE
        Qt6::Widgets
        Qt6::Network
        Core
        nlohmann_json::nlohmann_json
        tomlplusplus::tomlplusplus
)

# Устанавливаем путь для конфигурационного файла
configure_file(config.json ${CMAKE_BINARY_DIR}/bin/config_client.json COPYONLY)
configure_file(config.toml ${CMAKE_BINARY_DIR}/bin/config_client.toml COPYONLY)

# TODO fix later
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/send.png ${CMAKE_BINARY_DIR}/bin/images/send.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/user.png ${CMAKE_BINARY_DIR}/bin/images/user.png COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/styles/style.qss ${CMAKE_BINARY_DIR}/bin/styles/style.qss COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/styles/item_style.qss ${CMAKE_BINARY_DIR}/bin/styles/item_style.qss COPYONLY)

set_target_properties(ClientDesktop PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
