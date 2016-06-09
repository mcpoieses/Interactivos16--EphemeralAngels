#version 150
uniform sampler2DRect tex0;
uniform sampler2DRect uBodyIndexTex;
uniform int wingID;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{	
	vec4 texel0 = texture(tex0, texCoordVarying);
	vec4 texel1 = texture(uBodyIndexTex, texCoordVarying);
	
	if(wingID == int(255*texel1.r)){
		outputColor = vec4(texel0.rgb, texel0.a);
	}
	else{
		outputColor = vec4(texel0.rgb, 0);
	}
	
	//outputColor = vec4(texel0.rgba);
}
