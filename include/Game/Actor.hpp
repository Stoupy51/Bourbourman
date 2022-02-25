#pragma once

#include <utils.h>

#include <Engine/Mesh.hpp>
#include <Engine/Material.hpp>
#include <Engine/Transform.hpp>
#include <Engine/ResourceLoader.hpp>

class Map;

class Actor
{
protected:
	std::vector<Mesh*> m_meshes;
	std::vector<Material> m_materials;
	bool collision = true;
	Transform m_transform;
	Map* map;

public:
	Actor(Map* map);
	Actor(Map* map, const char* filename);

	/**
	 * @brief 
	 * 
	 * @param filename 
	 * @return true 
	 * @return false 
	 */
	bool loadOBJ(const char* filename);

	/**
	 * @brief Draw the object
	 */
	void draw() const;

	/**
	 * @brief Get the transform
	 * 
	 * @return Transform 
	 */
	inline Transform& getTransform() { return this->m_transform; }

	/**
	 * @brief Object collision getter and setter
	 */
	inline bool getCollision() { return this->collision; }
	inline void setCollision(bool collision) { this->collision = collision; }

	virtual void update(float deltaTime) { UNUSED(deltaTime); }
};