
# add_library(iconv::iconv1 SHARED IMPORTED GLOBAL)
# set_target_properties(iconv::iconv1 PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv
#     IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libiconv.lib
#     IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/libiconv2.dll)
# # add_library(iconv::iconv1bcc SHARED IMPORTED)
# # set_target_properties(iconv::iconv1bcc PROPERTIES
# #     IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libiconv-bcc.lib)
# add_library(iconv::charset SHARED IMPORTED GLOBAL)
# set_target_properties(iconv::charset PROPERTIES
#     sINTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv
#     IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcharset.lib
#     IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/libcharset1.dll)
# # add_library(iconv::charsetbcc SHARED IMPORTED)
# # set_target_properties(iconv::charsetbcc PROPERTIES
# #     IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcharset-bcc.lib)

# add_library(iconv INTERFACE IMPORTED GLOBAL)


# set_property(TARGET iconv PROPERTY
#     INTERFACE_LINK_LIBRARIES  iconv::charset iconv::iconv1)

# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/
#     DESTINATION ${PROJECT_BINARY_DIR}/)

# target_include_directories(iconv INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv)

add_library(iconv STATIC IMPORTED GLOBAL)
set_property(TARGET iconv PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libiconvStatic.lib)
target_include_directories(iconv INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include/)