#親ディレクトリ定義
set(LIB_DIR BREADLibrary/Source)

#ディレクトリ名
set(FBXLIB ${LIB_DIR}/FBX)
set(FNDLIB ${LIB_DIR}/FND)
set(FRAMELIB ${LIB_DIR}/FrameWork)
set(GRAPHICLIB ${LIB_DIR}/Graphics)
set(HLSLLIB ${LIB_DIR}/HLSL)
set(LOADERLIB ${LIB_DIR}/Loader)
set(MATHLIB ${LIB_DIR}/Math)
set(OSLIB ${LIB_DIR}/OS)

#読み込み
add_subdirectory(FBXLIB)
add_subdirectory(FNDLIB)
add_subdirectory(FRAMELIB)
add_subdirectory(GRAPHICLIB)
add_subdirectory(FBHLSLLIBXLIB)
add_subdirectory(LOADERLIB)
add_subdirectory(MATHLIB)
add_subdirectory(OSLIB)