
#include <Engine/Primitives.hpp>
#include <Engine/Material.hpp>
#include <Game/Wall.hpp>
#include <Game/Map.hpp>

Wall::Wall(Map* map, int type): Actor(map), type(type) {
	m_meshes.push_back(Primitives::cube());
	Material mat;
	switch (type) {
	case Metal:
		this->health = -1;
		mat.setDiffuseColor(glm::vec3(0.5f, 0.5f, 0.5f));
		mat.setSpecularColor(glm::vec3(0.6f, 0.6f, 0.6f));
		break;
	case Stone:
		mat.setDiffuseColor(glm::vec3(0.2f, 0.2f, 0.2f));
		mat.setSpecularColor(glm::vec3(0.3f, 0.3f, 0.3f));
		this->health = 3;
		break;
	case Wood:
		mat.setDiffuseColor(glm::vec3(0.86f, 0.72f, 0.52f));
		mat.setSpecularColor(glm::vec3(0.9f, 0.76f, 0.58f));
		this->health = 1;
		break;
	}
	m_materials.push_back(mat);
}

void Wall::onDestroy() {
	// summon un bonus ?
	// Destroy le wall
}

void Wall::removeHealth() {
	this->health -= 1;
	if (this->health == 0) {
		this->onDestroy();
	}
}