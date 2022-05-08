cmake_minimum_required(VERSION 3.6)
add_library(imgui STATIC
    imstb_truetype.h
    imstb_textedit.h
    imstb_rectpack.h
    imgui.h
    imgui.cpp
    imgui_widgets.cpp
    imgui_user.inl
    imgui_user.h
    imgui_tables.cpp
    imgui_internal.h
    imgui_draw.cpp
    imgui_demo.cpp
    imgui_impl_glfw.cpp
    imgui_impl_glfw.h
    imconfig.h
    stb_rect_pack.h
    stb_textedit.h
    stb_truetype.h
    widgets/color_picker.h
    widgets/color_picker.inl
    widgets/color_wheel.h
    widgets/color_wheel.inl
    widgets/dock.h
    widgets/dock.inl
    widgets/file_list.h
    widgets/file_list.inl
    widgets/gizmo.h
    widgets/gizmo.inl
    widgets/markdown.h
    widgets/markdown.inl
    widgets/memory_editor.h
    widgets/memory_editor.inl
    widgets/range_slider.h
    widgets/range_slider.inl
)
target_include_directories(imgui PUBLIC . widgets/)