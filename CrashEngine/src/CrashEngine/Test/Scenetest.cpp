#include "cepch.h"
#include "Scenetest.h"

#include "entt.hpp"//entt must be in cpp!!!
#include "CrashEngine/Scene/Components.h"


namespace CrashEngine {

    Scenetest::Scenetest()
    {
        entt::registry registry;
        auto view1 = registry.view<TransformComponent, MeshComponent>();
        view1.size();
    }
}