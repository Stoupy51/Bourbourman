#include <Engine/Lights.hpp>

using namespace std;
using namespace glm;

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->m_ambient = ambient;
    this->m_diffuse = diffuse;
    this->m_specular = specular;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular)
{
    this->m_direction = direction;
}

PointLight::PointLight(int index, float distance, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular)
{
    this->m_index = index;
    this->m_range = distance;
    this->m_position = position;
    this->m_enabled = false;
}

void DirectionalLight::SendToShader(const Shader& shader) const
{
	shader.Use();
	shader.SetUniformValue("_dirLight.direction", m_direction);
	shader.SetUniformValue("_dirLight.ambient", m_ambient);
	shader.SetUniformValue("_dirLight.diffuse", m_diffuse);
	shader.SetUniformValue("_dirLight.specular", m_specular);
}

void PointLight::SendToShader(const Shader& shader) const
{
	shader.Use();
    string lightIndexStr = to_string(m_index);
	shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].position").c_str(), m_position);
	shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].range").c_str(), m_range);
    if(m_enabled)
    {
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].ambient").c_str(), m_ambient);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].diffuse").c_str(), m_diffuse);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].specular").c_str(), m_specular);
    }
    else
    {
        vec3 noLight = vec3(0.0f);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].ambient").c_str(), noLight);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].diffuse").c_str(), noLight);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].specular").c_str(), noLight);
    }
}

void PointLight::reset()
{
    this->m_range = 10.0f;
    this->m_position = vec3(0.0f);
    this->m_enabled = false;
    this->m_ambient = vec3(0.0f);
    this->m_diffuse = vec3(0.0f);
    this->m_specular = vec3(0.0f);
}