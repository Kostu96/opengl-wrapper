add_subdirectory(glad)
add_subdirectory(stb)

set_target_properties(
    glad
    stb
    PROPERTIES
    FOLDER third_party
)
