#include "Mesh.h"
#include "Entity.h"
#include "EntityManager.h"
#include "LSystemParams.h"
#include "Stem.h"
#include "Leaf.h"
#include <vector>
#include <deque>

using Generator = std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>;

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
	std::vector<EntityId> stems;
	EntityId leavesId;

	void generateNewStems(EntityManager& manager);
	void updateStems();
	std::vector<EntityId> buildStemList(StemNode& root);
	float grow(const WorldTime& worldTime) const;
	void mergeToGeometry(StemNode* node);
	StemNode buildTree(EntityManager& manager, Generator& gen);
	Mesh initMesh();
	bool isEndStem(StemNode* node);
	//void addTerminalStem();

	float age, growthRate;
public:
	Tree(EntityManager& entityManager, Generator& gen);
	void act(const WorldTime& worldTime) override;
};