#include "Mesh.h"
#include "Entity.h"
#include "LSystemParams.h"

class Tree : public Entity {
private:
	StemNode* root;
public:
	Tree();
	Mesh initMesh();
	void act(const WorldTime& worldTime) override;
};