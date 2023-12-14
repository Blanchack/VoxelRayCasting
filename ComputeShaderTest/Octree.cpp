#include "Octree.h"
#include <iostream>
#include <stack>

Octree::Octree(){
	this->root = new OctreeNode(true, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Octree::~Octree()
{
	delete this->root;
}

void Octree::expand(){
	OctreeNode* newRoot = new OctreeNode(false, this->root->position, this->root->size * 2);
	newRoot->divide();

	if (this->root->hasData) {
		delete newRoot->children[0];
		newRoot->children[0] = this->root;
	}
	else
		delete this->root;

	this->root = newRoot;

}

void Octree::OctreeNode::divide(){
	bool childIsLeaf = this->size == 2.0f;
	float childSize = this->size / 2.0f;

	this->isDivided = true;
	//Orden anti-horario de abajo hacia arriba 

	this->children[0] = new OctreeNode(childIsLeaf, this->position, childSize);
	this->children[1] = new OctreeNode(childIsLeaf, this->position + glm::vec3(childSize, 0.0f, 0.0f), childSize);
	this->children[2] = new OctreeNode(childIsLeaf, this->position + glm::vec3(childSize, childSize, 0.0f), childSize);
	this->children[3] = new OctreeNode(childIsLeaf, this->position + glm::vec3(0.0f, childSize, 0.0f), childSize);

	this->children[4] = new OctreeNode(childIsLeaf, this->position + glm::vec3(0.0f, 0.0f, childSize), childSize);
	this->children[5] = new OctreeNode(childIsLeaf, this->position + glm::vec3(childSize, 0.0f, childSize), childSize);
	this->children[6] = new OctreeNode(childIsLeaf, this->position + glm::vec3(childSize, childSize, childSize), childSize);
	this->children[7] = new OctreeNode(childIsLeaf, this->position + glm::vec3(0.0f, childSize, childSize), childSize);
}

void Octree::insertVoxel(glm::vec3 voxelPos){
	while (!root->isInOctant(voxelPos))
		this->expand();

	OctreeNode* curr = root;

	while (!curr->isLeaf) {
		curr->hasData = true;
		for (int i = 0; i < 8; i++) {
			if (!curr->isDivided) 
				curr->divide();
			
			if (curr->children[i]->isInOctant(voxelPos)){
				curr = curr->children[i];
				break;
			}
		}
	}

	curr->hasData = true;
}

std::vector<Voxel> Octree::getSerializedData()
{
	std::vector<Voxel> data;
	data.push_back(root->toVoxel());
	root->serializedIndex = 0;

	std::stack<OctreeNode*> dfs;
	dfs.push(root);

	while (!dfs.empty()) {
		OctreeNode* curr = dfs.top(); dfs.pop();

		data[curr->serializedIndex].data.w = data.size();

		if (curr->isDivided) {
			for (int i = 0; i < 8; i++) {
				data.push_back(curr->children[i]->toVoxel());
				dfs.push(curr->children[i]);
				curr->children[i]->serializedIndex = data.size() - 1;
			}
		}
	}

	return data;
}

Octree::OctreeNode::OctreeNode(bool isLeaf, glm::vec3 position, float size)
	:isLeaf(isLeaf), position(position), size(size)
{
	this->hasData = false;
	this->isDivided = false;
	for (int i = 0; i < 8; i++)
		children[i] = nullptr;
}

Octree::OctreeNode::~OctreeNode()
{
	//std::cout << "Deleting voxel:" << this->position.x << " " << this->position.y << " " << this->position.z << " " << this->size << "\n";
	if (this->isDivided) {
		for (int i = 0; i < 8; i++)
			delete children[i];
	}
}

bool Octree::OctreeNode::isInOctant(glm::vec3 point)
{
	return point.x >= this->position.x && point.x < this->position.x + this->size &&
		point.y >= this->position.y && point.y < this->position.y + this->size &&
		point.z >= this->position.z && point.z < this->position.z + this->size;
}
