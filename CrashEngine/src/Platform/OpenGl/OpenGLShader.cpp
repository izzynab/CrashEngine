#include "cepch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CrashEngine {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that const char*'s .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			CE_CORE_ERROR("{0}", infoLog.data());
			CE_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that const char*'s .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			CE_CORE_ERROR("{0}", infoLog.data());
			CE_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CE_CORE_ERROR("{0}", infoLog.data());
			CE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::OpenGLShader(const char* vertexShader, const char* fragmentShader, const char* geometryShader)
	{
		std::string vPath = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";
		std::string fPath = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";
		std::string gPath = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";

		vPath += vertexShader;
		fPath += fragmentShader;
		gPath += geometryShader;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vPath);
			fShaderFile.open(fPath);
			gShaderFile.open(gPath);
			std::stringstream vShaderStream, fShaderStream,gShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			CE_CORE_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geometryCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment, geometry;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// geometry Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		GLint isVCompiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &isVCompiled);
		if (isVCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Vertex shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vertex); // Don't leak the shader.
			return;
		}

		GLint isFCompiled = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &isFCompiled);
		if (isFCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Vertex shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(fragment); // Don't leak the shader.
			return;
		}

		GLint isGCompiled = 0;
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &isGCompiled);
		if (isGCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(geometry, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Vertex shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(geometry); // Don't leak the shader.
			return;
		}

		// shader Program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		glAttachShader(m_RendererID, geometry);
		glLinkProgram(m_RendererID);
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}


	OpenGLShader::OpenGLShader(const char* vertexShader, const char* fragmentShader)
	{
		std::string vPath = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";
		std::string fPath = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";

		vPath += vertexShader;
		fPath += fragmentShader;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vPath);
			fShaderFile.open(fPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			CE_CORE_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		GLint isVCompiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &isVCompiled);
		if (isVCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Vertex shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vertex); // Don't leak the shader.
			return;
		}

		GLint isFCompiled = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &isFCompiled);
		if (isFCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Vertex shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(fragment); // Don't leak the shader.
			return;
		}

		// shader Program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		glLinkProgram(m_RendererID);
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(std::string name, int value)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i((projectionLoc), value);
	}

	void OpenGLShader::SetUniformFloat(std::string name, float value)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(projectionLoc, value);
	}

	void OpenGLShader::SetUniformVec2(std::string name, glm::vec2 vector)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(projectionLoc, 1, glm::value_ptr(vector));
	}

	void OpenGLShader::SetUniformVec3(std::string name, glm::vec3 vector)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(projectionLoc, 1, glm::value_ptr(vector));
	}

	void OpenGLShader::SetUniformVec4(std::string name, glm::vec4 vector)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(projectionLoc, 1, glm::value_ptr(vector));
	}

	void OpenGLShader::SetUniformMat3(std::string name, glm::mat4 matrix)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(std::string name, glm::mat4 matrix)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	uint32_t OpenGLShader::GetID()
	{
		return m_RendererID;
	}

	OpenGLComputeShader::OpenGLComputeShader(const char* computeShader)
	{
		std::string Path = "C:\\EngineDev\\CrashEngine\\CrashEngine\\src\\CrashEngine\\Renderer\\Shaders\\";

		Path += computeShader;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string computeCode;
		std::ifstream ShaderFile;
		// ensure ifstream objects can throw exceptions:
		ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			ShaderFile.open(Path);
			std::stringstream ShaderStream;
			// read file's buffer contents into streams
			ShaderStream << ShaderFile.rdbuf();
			// close file handlers
			ShaderFile.close();
			// convert stream into string
			computeCode = ShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			CE_CORE_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		const char* ShaderCode = computeCode.c_str();
		// 2. compile shaders
		unsigned int compute;
		// compute shader
		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &ShaderCode, NULL);
		glCompileShader(compute);
		
		GLint isCompiled = 0;
		glGetShaderiv(compute, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			//std::vector<char> errorLog(maxLength);
			std::string errorLog;
			errorLog.resize(maxLength, '0');
			glGetShaderInfoLog(compute, maxLength, &maxLength, &errorLog[0]);
			CE_CORE_ERROR("Compute shader error: {0}", errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(compute); // Don't leak the shader.
			return;
		}

		// shader Program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, compute);
		glLinkProgram(m_RendererID);
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(compute);

	}

	OpenGLComputeShader::~OpenGLComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLComputeShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	unsigned int OpenGLComputeShader::GetID()
	{
		return m_RendererID;
	}

	void OpenGLComputeShader::SetUniformInt(std::string name, int value)
	{
		int projectionLoc = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i((projectionLoc), value);
	}
}