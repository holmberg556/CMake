set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".l")
set(CMAKE_IMPORT_LIBRARY_SUFFIX ".l")
set(CMAKE_FIND_LIBRARY_PREFIXES "")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".l")

macro(__windows_compiler_orangec lang)
  set(CMAKE_${lang}_CREATE_WIN32_EXE "-Wg")
  set(CMAKE_${lang}_CREATE_CONSOLE_EXE "-Wc")
endmacro()