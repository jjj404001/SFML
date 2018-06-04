#ifndef RESOURCE_MANAGER_H
#define RESORCE_MANAGER_H

#include <unordered_map>
#include <fstream>
#include <sstream>
#include "Utilities.h"

template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string & pathsFile)
	{
		LoadPaths(pathsFile);
	}

	virtual ~ResourceManager() { PurgeResource(); }

	T* GetResource(const std::string & id)
	{
		auto res = Find(id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string & id) const
	{
		auto path = m_paths.find(id);
		return(path != m_paths.end() ? path->second : "");
	}

	bool RequireResource(const std::string & id)
	{
		auto res = Find(id);
		if (res)
		{
			++res->second;
			return true;
		}

		auto path = m_paths.find(id);
		if(path == m_paths.end())
			return false;
		T* resource = Load(path->second);
		if (!resource) { return false; }
		m_resources.emplace(id, std::make_pair(resource, 1));
		return true;
	}
	
	bool ReleaseResource(const std::string & id)
	{
		auto res = Find(id);
		if (!res) { return false; }
		--res->second;
		if (!res->second) { Unload(id); }
		return true;
	}

	void PurgeResource()
	{
		while(m_resources.begin() != m_resources.end())
		{
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	//Curiously Recurring Template Pattern
	T* Load(const std::string & path)
	{
		return static_cast<Derived*>(this)->Load(path);
	}

private:
	std::pair<T*, unsigned int> * Find(const std::string & id)
	{
		auto itr = m_resources.find(id);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string & id)
	{
		auto itr = m_resources.find(id);
		if (itr == m_resources.end()) return false;
		delete itr->second.first;
		m_resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string pathFile)
	{
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory() + pathFile);
		if (paths.is_open())
		{
			// <Example line>
			// PlayerSprite media/Textures/PlayerSheet.png

			std::string line;
			while(std::getline(paths, line))
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				m_paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}
		std::cerr << "! Failed loading the path file: " 
				<< pathFile << std::endl;
	}

	std::unordered_map<std::string, 
		std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;

};

#endif