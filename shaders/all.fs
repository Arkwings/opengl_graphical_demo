#version 330 core
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;

uniform bool dID;
uniform bool toon;

uniform float light_force;


in vec3 fragPos;
in vec3 norm;
in vec2 tex;
in vec3 lPos;
in vec3 lColor;
in vec3 ePos;
in vec4 fragPosLightSpace;


out vec4 fragColor;



void main()
{
    if(dID)
    {
        vec4 texColor = texture(diffuseTexture,tex).rgba;
        vec3 normbis = normalize(norm);
        vec3 lDir=normalize(lPos-fragPos);
        vec3 eDir=normalize(ePos-fragPos);

        float aPower=0.50f;  
        vec3 a=aPower*texColor.xyz;

        float diff=max(dot(lDir,normbis),0.0);
        vec3 d=diff*lColor;

        float sPower=0.9f;
        vec3 rDir=reflect(-lDir,normbis);
        vec3 halfwayDir = normalize(lDir + eDir); 
        float spec=pow(max(dot(normbis,halfwayDir),0.0),64);
        vec3 s=sPower*spec*lColor;

        vec3 lightingbis = a + d + s;

        if(!toon){
            float moy=(lightingbis.x+lightingbis.y+lightingbis.z)/3;
            if(moy<=0.2)lightingbis=lightingbis-moy+0.2;    
            else if(moy<=0.5)lightingbis=lightingbis-moy+0.5;    
            else if(moy<=0.8)lightingbis=lightingbis-moy+0.8;
            else if(moy<=1.1)lightingbis=lightingbis-moy+1.1;
        }
        
        vec3 lighting = lightingbis * texColor.rgb;

        if(texColor.a<0.02)
            discard;
        if(light_force<2.0)
            fragColor=vec4(1-lighting-0.2,texColor.a);
        else
            fragColor=vec4(lighting,texColor.a);
    }
    else
        if(light_force<2.0)
            fragColor=vec4(0.0,0.0,0.7,1.0);
        else     
            fragColor=vec4(0.0,0.0,0.0,1.0);

}