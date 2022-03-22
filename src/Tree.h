#include "Mesh.h"
#include "Entity.h"
#include "EntityManager.h"
#include "LSystemParams.h"

class Tree : public Entity {
private:
	StemNode root;

	void generateNewStems();
	void grow();
	StemNode buildTree(EntityManager& manager);
	Mesh initMesh();
public:
	Tree(EntityManager& manager);
	void act(const WorldTime& worldTime) override;
};