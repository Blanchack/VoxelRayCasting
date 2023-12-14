#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Voxel
{
	glm::vec4 pos;	//pos.x pos.y pos.z size
	glm::vec4 col;	// 0 0 0 0
	glm::ivec4 data;	//hasData isDivided isLeaf childrenIndex
};

class Octree {
public:
	class OctreeNode {
	public:
		bool isLeaf;
		bool hasData;
		bool isDivided;
		glm::vec3 position;
		float size;
		int serializedIndex;
		OctreeNode* children[8];

		OctreeNode(bool isLeaf, glm::vec3 position, float size);
		~OctreeNode();

		bool isInOctant(glm::vec3 point);

		Voxel toVoxel() {
			glm::ivec4 data(0, 0, 0, 0);

			if (this->hasData) data.x = 1;
			if (this->isDivided) data.y = 1;
			if (this->isLeaf) data.z = 1;

			Voxel voxel = { glm::vec4(position, size), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), data };

			return voxel;
		}

		void divide();
	};
private:
	OctreeNode* root;
	void expand();
public:
	Octree();
	~Octree();
	void insertVoxel(glm::vec3 voxelPos);
	std::vector<Voxel> getSerializedData();
	
};