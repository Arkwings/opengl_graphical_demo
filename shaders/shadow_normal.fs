#version 330 core

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;

uniform bool shadows;
uniform bool dID;
uniform bool toon;
uniform int gemColor;

uniform float light_force;

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



float ShadowCalculation(vec4 fragPosLightSpace,vec3 normbis,vec3 lDir,vec3 lPos,vec3 fragPos)
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
    float bias;
    //if(lDir)
    //bias= max(0.05 * (1.0 - dot(normbis, lDir)), 0.005);
    bias = 0.005*tan(acos(dot(normbis,lDir))); // cosTheta is dot( n,l ), clamped between 0 and 1
    bias = clamp(bias, 0,0.01);

    float shadow=0.0;

    // Check whether current frag pos is in shadow
    shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF  //ATTENTION CA POMPE CETTE MERDE ( baisser les valeurs de x et y en cas de bug)
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
     // petite application d ombrage selon la distance
     shadow -=4.0-sqrt((lPos.x-fragPos.x)*(lPos.x-fragPos.x)+(lPos.z-fragPos.z)*(lPos.z-fragPos.z)+(lPos.y-fragPos.y)*(lPos.y-fragPos.y))/35;
     //si derriere le far on applique simplement la distance pour gere l ombrage (pour supprimer l effet degueu)
     if(projCoords.z>1.0){
       shadow = -4.0+sqrt((lPos.x-fragPos.x)*(lPos.x-fragPos.x)+(lPos.z-fragPos.z)*(lPos.z-fragPos.z)+(lPos.y-fragPos.y)*(lPos.y-fragPos.y))/35;
   }
     //si sous le sol, pas d eclairage sauf ambiant
     if(lPos.y<0&&gemColor!=4)
     shadow=1.0;  // A REMETTRE EN FIN DE TEST

     return shadow; 
 }





 void main(){



    vec4 texColor= vec4(0,0,0,1);
    if(gemColor==0) 
    texColor = texture(diffuseTexture,fs_in.tex).rgba;
    //BLEU~VIOLET
    else if(gemColor==1) { 
        texColor = texture(diffuseTexture,fs_in.tex).rgba;
        texColor=vec4(texColor.r*1.2,texColor.g/2,texColor.b*1.5,texColor.a);
    }
    else if(gemColor==-1) {  
        texColor = texture(diffuseTexture,fs_in.tex).rgba;
        if(fs_in.lPos.y>0)
        texColor=vec4(1,0,3,texColor.a*5);
        else
        texColor=vec4(0,0,0,0);
    }
    //JAUNE~VERT
    else if(gemColor==2) {
        texColor = texture(diffuseTexture,fs_in.tex).gbra;
        texColor=vec4(texColor.r,texColor.g,texColor.b,texColor.a);
    }
    else if(gemColor==-2) {
        texColor = texture(diffuseTexture,fs_in.tex).gbra;
        if(fs_in.lPos.y>0)
        texColor=vec4(2,2,0.3,texColor.a*5);
        else
        texColor=vec4(0,0,0,0);
    }
    //ROUGE
    else if(gemColor==3) { 
        texColor = texture(diffuseTexture,fs_in.tex).brga;
        texColor=vec4(texColor.r*1.5,texColor.g/1.5,texColor.b/2,texColor.a);
    }
    else if(gemColor==-3) { 
        texColor = texture(diffuseTexture,fs_in.tex).brga;
        if(fs_in.lPos.y>0)
        texColor=vec4(1,0.1,0.1,texColor.a*5);
        else
        texColor=vec4(0,0,0,0);
    }
    //TEXT
    else if(gemColor==4){
        texColor=vec4(0.9372,0.8471,0.0274,1.0);
    }
    else if(gemColor==-4){
        texColor=vec4(0.0,0.0,0.0,1.0);
    }
    //GATE
    else if(gemColor==5){
        texColor=vec4(0.7372,0.6971,0.0174,1.0);
    }
    else if(gemColor==-20){
        texColor = texture(diffuseTexture,fs_in.tex/2+0.25);        
    }
    vec3 normbis=normalize(fs_in.norm);  //SS NORMAL MAP
    //vec3 normter=normalize(texture(normalMap,fs_in.tex).rgb * 2.0 -1.0); //avec NORMAP MAP ( MARCHE PAS ENCORE PUTIIIIIIIIIN)
    vec3 lDir=normalize(fs_in.lPos-fs_in.fragPos);
    vec3 eDir=normalize(fs_in.ePos-fs_in.fragPos);

    //AMBIANTE
    float aPower=0.60f/*light_force*/;  //set vers 0.15 quand non seul 
    vec3 a=aPower*texColor.xyz;
    //DIFFUSE
    float diff=max(dot(lDir,normbis),0.0);
    vec3 d;
    if(gemColor==0)
    d=diff*fs_in.lColor/*light_force/4*/;
    else
    d=diff*1.5*fs_in.lColor/*light_force/4*/;
    //SPECULAIRE
    float sPower=0.9f;
    vec3 rDir=reflect(-lDir,normbis);
    vec3 halfwayDir = normalize(lDir + eDir); 
    float spec=pow(max(dot(normbis,halfwayDir),0.0),64);
    vec3 s;
    if(gemColor==0)
    s=sPower*spec*fs_in.lColor/*light_force/4*/;
    else
    s=sPower*2*spec*fs_in.lColor/*light_force/4*/;
    //OMBRE
    float shadow = shadows? ShadowCalculation(fs_in.fragPosLightSpace,normbis,lDir,fs_in.lPos,fs_in.fragPos):0.0;       
    //FINALE
    vec3 lightingbis = (a + (1.0 - shadow) * (d + s));
    //TOON SHADING
    
    if(!toon){
        float moy=(lightingbis.x+lightingbis.y+lightingbis.z)/3;
        if(moy<=0.2)lightingbis=lightingbis-moy+0.2;    
        else if(moy<=0.5)lightingbis=lightingbis-moy+0.5;    
        else if(moy<=0.8)lightingbis=lightingbis-moy+0.8;
        else if(moy<=1.1)lightingbis=lightingbis-moy+1.1;
    }

    vec3 lighting = lightingbis * texColor.rgb;
    if(dID==true){
        if(texColor.a<0.02)discard;
        if(gemColor!=-4&&light_force<2.0)
            fragColor= vec4(1-lighting-0.2,texColor.a);
        else
            fragColor= vec4(lighting,texColor.a);
    }  






    //SS ECLAIRAGE
    //if(dID==1)
    //color=vec4(texColor.x,texColor.y-texColor.y/2.5,texColor.z-texColor.z/1.5,1.0);

    //MIXER AVEC LA LUMIERE
    if(dID==false)
        if(light_force<2.0){
            vec4 backColor=mix(vec4(fs_in.lColor,texColor.w),vec4(texColor.x,texColor.y-texColor.y/2.5,texColor.z-texColor.z/1.5,texColor.w),0.9);
            fragColor=vec4(1-backColor.xyz-0.2,backColor.w);
        }
        else
            fragColor=mix(vec4(fs_in.lColor,texColor.w),vec4(texColor.x,texColor.y-texColor.y/2.5,texColor.z-texColor.z/1.5,texColor.w),0.9);

}