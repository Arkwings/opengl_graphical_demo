#include <limits.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <assimp/Importer.hpp>    
#include <assimp/scene.h>    
#include <assimp/postprocess.h> 

using namespace std;


#define INVALID_UNIFORM_LOCATION 0xffffffff
#define SNPRINTF snprintf







struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{        
    vec3 Direction;

    DirectionalLight()
    {
        Direction = vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    vec3 Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

struct SpotLight : public PointLight
{
    vec3 Direction;
    float Cutoff;

    SpotLight()
    {
        Direction = vec3(0.0f, 0.0f, 0.0f);
        Cutoff = 0.0f;
    }
};

class SkinningTechnique 
{
public:

    static const uint MAX_POINT_LIGHTS = 2;
    static const uint MAX_SPOT_LIGHTS = 2;
    static const uint MAX_BONES = 100;

    SkinningTechnique();

    bool Init(GLuint shaderID);  //voir de quoi ca depdndati apres avoir enlevé le virtual 

    void SetM(const mat4& M);
    void SetV(const mat4& V);
    void SetP(const mat4& P);
    void SetLightSpaceMatrix(const mat4& lightSpaceMatrix);
    void SetColorTextureUnit(uint TextureUnit);
    void SetDirectionalLight(const DirectionalLight& Light);
    void SetPointLights(uint NumLights, const PointLight* pLights);
    void SetSpotLights(uint NumLights, const SpotLight* pLights);
    void SetEyeWorldPos(const vec3& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
    void SetBoneTransform(uint Index, const mat4& Transform);

private:

    GLuint m_WorldMatrixLocation;
    GLuint m_ViewMatrixLocation;
    GLuint m_ProjectionMatrixLocation;
    GLuint m_LightSpaceMatrixLocation;
    GLuint m_colorTextureLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_numPointLightsLocation;
    GLuint m_numSpotLightsLocation;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } m_dirLightLocation;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        GLuint Direction;
        GLuint Cutoff;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } m_spotLightsLocation[MAX_SPOT_LIGHTS];
    
    GLuint m_boneLocation[MAX_BONES];
};











SkinningTechnique::SkinningTechnique()
{  
}


bool SkinningTechnique::Init(GLuint shaderID)
{
    m_WorldMatrixLocation = glGetUniformLocation(shaderID,"M");
    m_ViewMatrixLocation = glGetUniformLocation(shaderID,"V");
    m_ProjectionMatrixLocation = glGetUniformLocation(shaderID,"P");
    m_LightSpaceMatrixLocation = glGetUniformLocation(shaderID,"lightSpaceMatrix");
    m_colorTextureLocation = glGetUniformLocation(shaderID,"gColorMap");
    m_eyeWorldPosLocation = glGetUniformLocation(shaderID,"gEyeWorldPos");
    m_dirLightLocation.Color = glGetUniformLocation(shaderID,"gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = glGetUniformLocation(shaderID,"gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.Direction = glGetUniformLocation(shaderID,"gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(shaderID,"gDirectionalLight.Base.DiffuseIntensity");
    m_matSpecularIntensityLocation = glGetUniformLocation(shaderID,"gMatSpecularIntensity");
    m_matSpecularPowerLocation = glGetUniformLocation(shaderID,"gSpecularPower");
    m_numPointLightsLocation = glGetUniformLocation(shaderID,"gNumPointLights");
    m_numSpotLightsLocation = glGetUniformLocation(shaderID,"gNumSpotLights");

    // if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
    //     m_WVPLocation == INVALID_UNIFORM_LOCATION ||
    //     m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
    //     m_colorTextureLocation == INVALID_UNIFORM_LOCATION ||
    //     m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
    //     m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
    //     m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
    //     m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
    //     m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
    //     m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
    //     m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
    //     m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION) {
    //         printf("\nc est pas bon! 1\n");
    //     return false;
    // }

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = glGetUniformLocation(shaderID,Name);

        // if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
        //     m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
        //     printf("\nc est pas bon! 2\n");
        //     return false;
        // }
    }

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        m_spotLightsLocation[i].Color = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        m_spotLightsLocation[i].AmbientIntensity = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        m_spotLightsLocation[i].Position = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        m_spotLightsLocation[i].Direction = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        m_spotLightsLocation[i].Cutoff = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        m_spotLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        m_spotLightsLocation[i].Atten.Constant = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        m_spotLightsLocation[i].Atten.Linear = glGetUniformLocation(shaderID,Name);

        SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        m_spotLightsLocation[i].Atten.Exp = glGetUniformLocation(shaderID,Name);

        // if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
        //     m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
        //     printf("\nc est pas bon! 3\n");
        //     return false;
        // }
    }
    printf("\n\nelements=%lu\n\n",ARRAY_SIZE_IN_ELEMENTS(m_boneLocation));
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
        m_boneLocation[i] = glGetUniformLocation(shaderID,Name);
    }

    return true;
}



void SkinningTechnique::SetM(const mat4& M)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, &M[0][0]);
}
void SkinningTechnique::SetV(const mat4& V)
{
    glUniformMatrix4fv(m_ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);
}
void SkinningTechnique::SetP(const mat4& P)
{
    glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, &P[0][0]);
}
void SkinningTechnique::SetLightSpaceMatrix(const mat4& lightSpaceMatrix)
{
    glUniformMatrix4fv(m_LightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

void SkinningTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureLocation, TextureUnit);
}


void SkinningTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    vec3 Direction = Light.Direction;
    Direction=normalize(Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}


void SkinningTechnique::SetEyeWorldPos(const vec3& EyeWorldPos)
{
    glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}


void SkinningTechnique::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(m_matSpecularIntensityLocation, Intensity);
}


void SkinningTechnique::SetMatSpecularPower(float Power)
{
    glUniform1f(m_matSpecularPowerLocation, Power);
}


void SkinningTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
    glUniform1i(m_numPointLightsLocation, NumLights);
    
    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}

void SkinningTechnique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
    glUniform1i(m_numSpotLightsLocation, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_spotLightsLocation[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        vec3 Direction = pLights[i].Direction;
        Direction=normalize(Direction);
        glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
        glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(radians(pLights[i].Cutoff)));
        glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_spotLightsLocation[i].Atten.Linear,   pLights[i].Attenuation.Linear);
        glUniform1f(m_spotLightsLocation[i].Atten.Exp,      pLights[i].Attenuation.Exp);
    }
}


void SkinningTechnique::SetBoneTransform(uint Index, const mat4& Transform)
{
    //cout<<Index<<"  "<<endl;
    assert(Index < MAX_BONES);
    // cout<<Transform[0][0]<<"  "<<Transform[0][1]<<"  "<<Transform[0][2]<<"  "<<Transform[0][3]<<endl;
    // cout<<Transform[1][0]<<"  "<<Transform[1][1]<<"  "<<Transform[1][2]<<"  "<<Transform[1][3]<<endl;
    // cout<<Transform[2][0]<<"  "<<Transform[2][1]<<"  "<<Transform[2][2]<<"  "<<Transform[2][3]<<endl;
    // cout<<Transform[3][0]<<"  "<<Transform[3][1]<<"  "<<Transform[3][2]<<"  "<<Transform[3][3]<<endl<<endl;
    glUniformMatrix4fv(m_boneLocation[Index], 1, GL_FALSE, &Transform[0][0]);       
}