#pragma once 

namespace cwt
{
    class mesh 
    {
    public:
        mesh(GLfloat* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices)
        {
            create(vertices, indices, num_of_vertices, num_of_indices);
        }

        ~mesh() 
        {
            clear();
        }

        void create(GLfloat* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices)
        {
            m_index_count = num_of_indices;

            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            glGenBuffers(1, &m_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*num_of_indices, indices, GL_STATIC_DRAW);

            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*num_of_vertices, vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            
            glBindVertexArray(0);

        }
        void render()
        {
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        void clear()
        {
            if (m_ibo != 0) {
                glDeleteBuffers(1, &m_ibo);
                m_ibo = 0;
            }
            if (m_vbo != 0) {
                glDeleteBuffers(1, &m_vbo);
                m_vbo = 0;
            }
            if (m_vao != 0) {
                glDeleteVertexArrays(1, &m_vao);
                m_vao = 0;
            }
            m_index_count = 0;
        }
    private:
        GLuint m_vao{0};
        GLuint m_vbo{0};
        GLuint m_ibo{0};
        GLsizei m_index_count{0};
    };


    std::unique_ptr<mesh> create_mesh()
    {
        unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
        };

        GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f, // 0
            0.0f, -1.0f, 1.0f, // 1
            1.0f, -1.0f, 0.0f, // 2
            0.0f, 1.0f, 0.0f // 3
        };
        return std::make_unique<mesh>(vertices, indices, 12, 12);
    }
} // namespace cwt


