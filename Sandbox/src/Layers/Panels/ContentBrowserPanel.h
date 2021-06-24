#pragma once
#include <filesystem>

namespace CrashEngine {
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

		bool IconButton(const char* icon, const char* filename = nullptr, float scale = 1.f);
	private:
		std::filesystem::path m_CurrentDirectory;
		std::string m_AssetPath;

	};
}