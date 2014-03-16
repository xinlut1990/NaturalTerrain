#include "myMath.h"

class Light
{
public:
	Light();
	Light(vec4 position, color intensityDiff, color intensitySpec);
	~Light();
	vec4 getPosition();
	color getIntensityDiff();
	color getIntensitySpec();

private:
	vec4 position;
	color intensityDiff;
	color intensitySpec;
};