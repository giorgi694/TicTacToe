set (
    PROJECT_RESOURCES
    "res/font-ms-dos.ttf"
)

set (RES_FIXED_PATH)
foreach (resource IN LISTS PROJECT_RESOURCES)
    list (APPEND RES_FIXED_PATH "${PROJECT_SOURCE_DIR}/${resource}")
endforeach ()

if (NOT ("${RES_FIXED_PATH}" STREQUAL ""))
    embed_resources (
        TARGET
            ${PROJECT_ID}
        RESOURCES
            ${RES_FIXED_PATH}
    )
endif ()
