#pragma once

//#include "CrashEngine/Scene/Material.h"
#include "CrashEngine/Scene/Mesh.h"

namespace CrashEngine {

	class MaterialPanel
	{
	public:
		MaterialPanel();

		void SetEntity(Mesh& mesh);

		void OnImGuiRender();

	private:
		std::shared_ptr<Mesh> m_Mesh;

		std::shared_ptr<Texture2D> albedo_loaded;
		std::shared_ptr<Texture2D> metallic_loaded;
		std::shared_ptr<Texture2D> roughness_loaded;

		std::shared_ptr<TextureCreator> texcreator;
	};

}
