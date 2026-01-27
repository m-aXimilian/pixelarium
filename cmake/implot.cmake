include(FetchContent)
FetchContent_Declare(
  implot
  URL https://github.com/epezent/implot/archive/refs/tags/v0.17.zip
  DOWNLOAD_EXTRACT_TIMESTAMP 1)

FetchContent_MakeAvailable(implot)

FetchContent_GetProperties(implot SOURCE_DIR IMPLOT_DIR)
