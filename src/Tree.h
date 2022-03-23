#include "Mesh.h"
#include "Entity.h"
#include "EntityManager.h"
#include "LSystemParams.h"
#include "Stem.h"
#include <vector>

struct TerminalStem {
	StemNode* node;
	bool isVisited;
};

class Tree : public Entity {
	static const int maxAge = 1.0;
private:
	EntityManager& manager;

	StemNode root;
	std::vector<TerminalStem> terminalStems;
	void generateNewStems(EntityManager& manager);
	void grow();
	void mergeToGeometry(Stem& stem);
	StemNode buildTree(EntityManager& manager);
	Mesh initMesh();
	bool isEndStem(StemNode* node);
	//void addTerminalStem();

	float age, growthRate;
public:
	Tree(EntityManager& entityManager);
	void act(const WorldTime& worldTime) override;
};