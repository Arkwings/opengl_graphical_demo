#version 330 core

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform bool shadows;
uniform bool dID;

in VS_OUT 
{
	vec3 fragPos;
	vec3 norm;
	vec2 tex;
	vec3 lPos;
	vec3 lColor;
	vec3 ePos;
	vec4 fragPosLightSpace;
} 
fs_in;

out vec4 fragColor;


float ShadowCalculation(vec4 fragPosLightSpace,vec3 normbis,vec3 lDir)
{
  // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normbis, lDir)), 0.005);
    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    //float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
     if(projCoords.z > 1.0){
        if(shadow>0)
         shadow += (1.0 - projCoords.z)*10;
     }
        
    return shadow; 
}


void main(){

	vec4 texColor = texture(diffuseTexture,fs_in.tex).rgba;
	vec3 normbis=normalize(fs_in.norm);
	vec3 lDir=normalize(fs_in.lPos-fs_in.fragPos);
	vec3 eDir=normalize(fs_in.ePos-fs_in.fragPos);

    //AMBIANTE
    float aPower=0.15f;  //set vers 0.15 quand non seul 
    vec3 a=aPower*texColor.xyz;
    //DIFFUSE
    float diff=max(dot(lDir,normbis),0.0);
    vec3 d=diff*fs_in.lColor;
    //SPECULAIRE
    float sPower=0.5f;
    vec3 rDir=reflect(-lDir,normbis);
    vec3 halfwayDir = normalize(lDir + eDir); 
    float spec=pow(max(dot(normbis,halfwayDir),0.0),64);
    vec3 s=sPower*spec*fs_in.lColor;
    //OMBRE
    float shadow = shadows? ShadowCalculation(fs_in.fragPosLightSpace,normbis,lDir):0.0;       
    //FINALE
    vec3 lighting = (a + (1.0 - shadow) * (d + s)) * texColor.rgb;    
    if(dID==true)
        fragColor=vec4(lighting,texColor.a);  

    




    
    //SS ECLAIRAGE
    //if(dID==1)
    //color=vec4(texColor.x,texColor.y-texColor.y/2.5,texColor.z-texColor.z/1.5,1.0);

    //MIXER AVEC LA LUMIERE
    if(dID==false)
      fragColor=mix(vec4(fs_in.lColor,1.0),vec4(texColor.x,texColor.y-texColor.y/2.5,texColor.z-texColor.z/1.5,1.0),0.9);

}