#ifndef MATERIAL
#define MATERIAL

class BasicModel;

class Material {
	friend class BasicModel;

private:
	float diffuse, specular, specular_power, reflection, transparency, refraction_radio, absorbance;

public:
	Material(): diffuse(0), specular(0), specular_power(0), reflection(0), transparency(0), refraction_radio(0), absorbance(0) {}

	Material(float diffuse, float specular, float specular_power, float reflection, float transparency,
	         float refraction_radio, float absorbance): diffuse(diffuse), specular(specular), specular_power(specular_power),
		reflection(reflection), transparency(transparency), refraction_radio(refraction_radio), absorbance(absorbance) {}
};

#endif