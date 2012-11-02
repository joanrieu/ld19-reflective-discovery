add_definitions(-I/mingw/lib/wx/include/msw-unicode-2.9 -I/mingw/include/wx-2.9 -D_LARGEFILE_SOURCE=unknown -DWXUSINGDLL -D__WXMSW__ -mthreads)

add_executable(
    Discovery
    GameApp.cpp GameFrame.cpp
    GamePanel.cpp GP_Title.cpp GP_Game.cpp
    GGame.cpp
)

target_link_libraries(
    Discovery
    -L/mingw/lib   -Wl,--subsystem,windows -mwindows -lwx_mswu_adv-2.9 -lwx_mswu_core-2.9 -lwx_baseu-2.9
)