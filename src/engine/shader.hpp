#pragma once

namespace cwt
{
    static const char* vertex_shader_file = "src/engine/shaders/shader.vert";
    static const char* fragment_shader_file = "src/engine/shaders/shader.frag";

    std::string read_file(const char* path)
    {
        std::string content; 
        std::ifstream filestream(path, std::ios::in);

        if (!filestream.is_open()){
            printf("Error reading file %s!", path);
            return std::string{""};
        }

        std::string line = "";
        while(!filestream.eof())
        {
            std::getline(filestream, line);
            content.append(line + '\n');
        }
        filestream.close();
        return content;
    }

    class shader 
    {
    public:
        shader(const char* vertex_file, const char* fragment_file)
        {
            const std::string vertex_code = read_file(vertex_file);
            const std::string fragment_code = read_file(fragment_file);

            m_id = glCreateProgram();
            if(!m_id) {
                printf("Error creating shader program!\n"); 
                exit(1);
            }

            add(m_id, vertex_code.c_str(), GL_VERTEX_SHADER);
            add(m_id, fragment_code.c_str(), GL_FRAGMENT_SHADER);

            GLint result = 0;
            GLchar log[1024] = {0};

            glLinkProgram(m_id);
            glGetProgramiv(m_id, GL_LINK_STATUS, &result);
            if (!result) {
                glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
                printf("Error linking program! %s\n", log);
                return;
            }

            glValidateProgram(m_id);
            glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
            if (!result) {
                glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
                printf("Error validating program! %s\n", log);
                return;
            }

            m_model = glGetUniformLocation(m_id, "model");
            m_projection = glGetUniformLocation(m_id, "projection");
        }
        ~shader()
        {
            clear();
        }

        GLuint get_projection_location()
        {
            return m_projection;
        }
        GLuint get_model_location()
        {
            return m_model;
        }

        void use()
        {
            glUseProgram(m_id);
        }
        void clear()
        {
            if (m_id != 0) {
                glDeleteProgram(m_id);
                m_id = 0;
            }
            m_model = 0;
            m_projection = 0;
        }

    private:
        void add(GLuint program, const char* shader_code, GLenum type)
        {
            GLuint current_shader = glCreateShader(type);

            const GLchar* code[1];
            code[0] = shader_code;

            GLint code_length[1];
            code_length[0] = strlen(shader_code);

            glShaderSource(current_shader, 1, code, code_length);
            glCompileShader(current_shader);

            GLint result = 0;
            GLchar log[1024] = {0};

            glGetShaderiv(current_shader, GL_COMPILE_STATUS, &result);
            if (!result) {
                glGetShaderInfoLog(current_shader, sizeof(log), NULL, log);
                printf("Error compiling %d  shader! %s\n", type, log);
                return;
            }

            glAttachShader(program, current_shader);
        }

    private:
        GLuint m_id{0};
        GLuint m_projection{0};
        GLuint m_model{0};
    };
 
} // namespace cwt


