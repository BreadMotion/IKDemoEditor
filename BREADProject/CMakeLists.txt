#ソースコード追加
set(MAIN_SOURCE ${APP_NAME}/)

#実行ファイルを作成
add_executable(${APP_NAME} ${MAIN_SOURCE})

#実行ファイルの場所を指定
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/Build/bin")

#ライブラリのリンク
target_link_libraries(${APP_NAME} PRIVATE Library)
target_include_directories(${APP_NAME} PRIVATE "PROEJCT_SOURCE_DIR"/lib)