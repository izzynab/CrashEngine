#include "cepch.h"
#include "Entity.h"

namespace CrashEngine {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}