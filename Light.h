#pragma once

#include "myMath.h"

class Light
{
public:
	Light();
	Light(vec4 position, color intensityDiff, color intensitySpec);
	~Light();
	vec4 getPosition() const;
	color getIntensityDiff() const;
	color getIntensitySpec() const;

private:
	vec4 position;
	color intensityDiff;
	color intensitySpec;
};