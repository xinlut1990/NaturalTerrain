#include "Light.h"

Light::Light(): position(0.0f, 0.0f, 0.0f, 1.0f), 
	intensityDiff(0.0f, 0.0f, 0.0f, 1.0f),
	intensitySpec(0.0f, 0.0f, 0.0f, 1.0f)
{

}

Light::Light(vec4 position, color intensityDiff, color intensitySpec): position(position),
	intensityDiff(intensityDiff), 
	intensitySpec(intensitySpec)
{	

}

Light::~Light()
{
}

vec4 Light::getPosition()
{
	return position;
}

color Light::getIntensityDiff()
{
	return intensityDiff;
}

color Light::getIntensitySpec()
{
	return intensitySpec;
}