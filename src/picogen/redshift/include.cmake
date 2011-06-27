set (sources ${sources}
        ${pwd}/src/constants.cc
        ${pwd}/src/jobfile.cc
        ${pwd}/src/quatsch-integration.cc
        ${pwd}/src/get_compilation_info.cc
        ${pwd}/src/save_openexr.cc

        ${pwd}/src/backgrounds/pss-adapter.cc
        ${pwd}/src/backgrounds/preetham-shirley-smits/atmconstants.cc
        ${pwd}/src/backgrounds/preetham-shirley-smits/sunconstants.cc
        ${pwd}/src/backgrounds/preetham-shirley-smits/sunsky.cc
        ${pwd}/src/backgrounds/preetham-shirley-smits/tospectrum.cc

        ${pwd}/src/basictypes/bsdf.cc
        ${pwd}/src/basictypes/differentialgeometry.cc
        ${pwd}/src/basictypes/intersection.cc
        ${pwd}/src/basictypes/material.cc
        ${pwd}/src/basictypes/scene.cc
        ${pwd}/src/basictypes/spectrum.cc
        ${pwd}/src/basictypes/volume.cc

        ${pwd}/src/bxdf/brdftobtdf.cc
        ${pwd}/src/bxdf/lambertian.cc
        ${pwd}/src/bxdf/mirror.cc

        ${pwd}/src/cameras/cubemapface.cc
        ${pwd}/src/cameras/cylindrical.cc
        ${pwd}/src/cameras/pinhole.cc

        ${pwd}/src/integrators/emission.cc
        ${pwd}/src/integrators/path.cc
        ${pwd}/src/integrators/redshift.cc
        ${pwd}/src/integrators/single-scattering.cc
        ${pwd}/src/integrators/whitted.cc

        ${pwd}/src/interaction/passivecommandprocessor.cc
        ${pwd}/src/interaction/sdlcommandprocessor.cc

        ${pwd}/src/material/leaf0.cc
        ${pwd}/src/material/matte.cc

        ${pwd}/src/primitives/boundinstance.cc
        ${pwd}/src/primitives/bvh.cc
        ${pwd}/src/primitives/closedsphere.cc
        ${pwd}/src/primitives/forest.cc
        ${pwd}/src/primitives/horizonplane.cc
        ${pwd}/src/primitives/instance.cc
        ${pwd}/src/primitives/lazyquadtree.cc
        ${pwd}/src/primitives/list.cc
        ${pwd}/src/primitives/lsystemtree.cc
        ${pwd}/src/primitives/triangle.cc
        ${pwd}/src/primitives/trianglebvh.cc
        ${pwd}/src/primitives/waterplane.cc

        ${pwd}/src/rendertargets/colorrendertarget.cc
        ${pwd}/src/rendertargets/sdlrendertarget.cc

        ${pwd}/src/shapes/closedsphere.cc

        ${pwd}/src/texture/image.cc

        ${pwd}/src/volume/exponential.cc
        ${pwd}/src/volume/homegeneous.cc
        ${pwd}/src/volume/list.cc
)
include_directories ("${pwd}/include/")
