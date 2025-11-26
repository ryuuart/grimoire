vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ryuuart/touchdesigner-custom-operators
    REF main
    SHA512 e46f055a89da5ec482e0de0d3e19d54cbe00dd698158b4f9431bfbcac935928fc33d3d1eac6dcc7ddd3becf46365fd97aa3c26a6cf6045030e66055bfeab360b
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH "lib/cmake/touchdesigner")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
