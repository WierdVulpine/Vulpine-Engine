#include "Model.h"

void Model::AddAnimation(Animation& anAnimation)
{
	mySkeleton.Animations.insert({ anAnimation.Name, anAnimation });
}
