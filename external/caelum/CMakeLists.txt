add_library(caelum
        src/Astronomy.cpp src/BrightStarCatalogue.cpp src/CaelumDefaultTypeDescriptorData.cpp
        src/CaelumPlugin.cpp src/CaelumPrecompiled.cpp src/CaelumScriptTranslator.cpp src/CaelumSystem.cpp src/CameraBoundElement.cpp
        src/CloudSystem.cpp src/DepthComposer.cpp src/FastGpuParamRef.cpp src/FlatCloudLayer.cpp
        src/GroundFog.cpp src/ImageStarfield.cpp src/InternalUtilities.cpp src/Moon.cpp src/PointStarfield.cpp
        src/PrecipitationController.cpp src/SkyDome.cpp src/SkyLight.cpp src/SphereSun.cpp src/TypeDescriptor.cpp
        src/UniversalClock.cpp)

target_include_directories(caelum PUBLIC include)


