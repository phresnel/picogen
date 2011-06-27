set (sources ${sources}
        ${pwd}/src/object.cc
        ${pwd}/src/save_load.cc
        ${pwd}/src/scene.cc
        ${pwd}/src/to_redshift/object_to_redshift.cc
        ${pwd}/src/to_redshift/redshift_file_to_redshift.cc
)
include_directories ("${pwd}/include/")
