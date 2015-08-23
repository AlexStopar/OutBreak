#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform bool isAnimating;
uniform bool isRainbow;
uniform bool isScrolling;
uniform float frameNumberMax;
uniform float currentFrameNumber;
uniform vec3 spriteColor;
uniform float rainbowOffset;
uniform float rainbowColorAlpha;
uniform float scrollTime;
uniform int eyeColor;

const float rainbowMainAlpha = 0.5;
const float eyeColorAlpha = 0.3;

struct MixColor
{
	vec4 color;
	float min;
	float max;
};

MixColor colorArray[7] = MixColor[7](
	MixColor(vec4(1.0, 0.0, 0.0, 1.0), 0.0, 1.0/7.0),
	MixColor(vec4(1.0, 0.647059, 0.0, 1.0), 1.0/7.0, 2.0/7.0),
	MixColor(vec4(1.0, 1.0, 0.0, 1.0), 2.0/7.0, 3.0/7.0),
	MixColor(vec4(0.0, 1.0, 0.0, 1.0), 3.0/7.0, 4.0/7.0),
	MixColor(vec4(0.0, 0.0, 1.0, 1.0), 4.0/7.0, 5.0/7.0),
	MixColor(vec4(0.294118, 0.0, 0.509804, 1.0), 5.0/7.0, 6.0/7.0),
	MixColor(vec4(0.933333, 0.509804, 0.933333, 1.0), 6.0/7.0, 1.0)
);

vec4 eyeColors[5] = vec4[5](
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(0.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 0.647059, 0.0, 1.0),
	vec4(1.0, 0.0, 0.0, 1.0)
);

vec4 invincibleRainbow(vec4 color, vec2 tex)
{

	for(int i = 0; i < colorArray.length(); ++i)
	{
		colorArray[i].min += rainbowOffset;
		if(colorArray[i].min > 1.0) colorArray[i].min -= 1.0;
		colorArray[i].max += rainbowOffset;
		if(colorArray[i].max > 1.0) colorArray[i].max -= 1.0;

		colorArray[i].color = mix(colorArray[i].color, 
		colorArray[(i+1) % colorArray.length()].color, rainbowColorAlpha);
		if(color.a > 0.0 && 
		((tex.x > colorArray[i].min && tex.x < colorArray[i].max && colorArray[i].min < colorArray[i].max)
		|| (colorArray[i].min > colorArray[i].max && (tex.x < colorArray[i].max || tex.x > colorArray[i].min))))
		{
			color = mix(color, colorArray[i].color, rainbowMainAlpha);
		}
	}
	
	
	return color;
}

vec4 Animate()
{
	
	return vec4(spriteColor, 1.0) * texture(image, vec2((TexCoords.x/frameNumberMax) + 
		(currentFrameNumber/frameNumberMax), TexCoords.y));
}

vec4 Scroll()
{
	return vec4(spriteColor, 1.0) * texture( image, vec2(TexCoords.x + scrollTime, TexCoords.y) );
}

void main()
{    
	if(isAnimating) color = Animate();
	else if(isScrolling) color = Scroll();
    else color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
	if(isRainbow && color.a > 0.0) color = invincibleRainbow(color, TexCoords);
	if(eyeColor > 0 && color.a > 0.0) color = mix(color, eyeColors[eyeColor - 1], eyeColorAlpha); 
}  