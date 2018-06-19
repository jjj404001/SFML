#ifndef ANIM_DIRECRIONAL_H
#define ANIM_DIRECTIONAL_H

#include "Anim_Base.h"

class Anim_Directional : public Anim_Base
{
protected:
	void FrameStep();
	void CropSprite();
	void ReadIn(std::stringstream & stream);
};

#endif