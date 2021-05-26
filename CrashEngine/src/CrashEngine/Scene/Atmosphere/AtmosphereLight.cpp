#include "cepch.h"

#include "AtmosphereLight.h"

static double kDefaultLambdas[] = { kLambdaR, kLambdaG, kLambdaB };
static double kDefaultLuminanceFromRadiance[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };

namespace CrashEngine {

	AtmosphereLight::AtmosphereLight()
	{
		//todo: make AtmosphereLight
	}

	AtmosphereLight::~AtmosphereLight()
	{
		m_absorption_density.clear();
	}

	void AtmosphereLight::initialize(int num_scattering_orders)
	{
		
	}

	void AtmosphereLight::bind_rendering_uniforms(Shader* shader)
	{

	}

	void AtmosphereLight::convert_spectrum_to_linear_srgb(double& r, double& g, double& b)
	{

	}

	double AtmosphereLight::coeff(double lambda, int component)
	{
		return 0.0;
	}

	void AtmosphereLight::bind_compute_uniforms(Shader* shader, double* lambdas, double* luminance_from_radiance)
	{

	}

	void AtmosphereLight::bind_density_layer(Shader* shader, DensityProfileLayer* layer)
	{

	}

	void AtmosphereLight::sky_sun_radiance_to_luminance(glm::vec3& sky_spectral_radiance_to_luminance, glm::vec3& sun_spectral_radiance_to_luminance)
	{

	}

	void AtmosphereLight::swap(Texture2D** arr)
	{
	}
	glm::vec3 AtmosphereLight::to_vector(const std::vector<double>& wavelengths, const std::vector<double>& v, double lambdas[], double scale)
	{
		return glm::vec3();
	}
	glm::mat4 AtmosphereLight::to_matrix(double arr[])
	{
		return glm::mat4();
	}
	double AtmosphereLight::cie_color_matching_function_table_value(double wavelength, int column)
	{
		return 0.0;
	}
	double AtmosphereLight::interpolate(const std::vector<double>& wavelengths, const std::vector<double>& wavelength_function, double wavelength)
	{
		return 0.0;
	}
	void AtmosphereLight::compute_spectral_radiance_to_luminance_factors(const std::vector<double>& wavelengths, const std::vector<double>& solar_irradiance, double lambda_power, double& k_r, double& k_g, double& k_b)
	{
	}
}