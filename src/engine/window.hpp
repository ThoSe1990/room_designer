#pragma once 

namespace cwt
{
    
    class window 
    {
    public:
        int init()
        {
            if (!glfwInit())
            {
                printf("GLFW initialisation failed!");
                glfwTerminate();
                return 1;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            m_window = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);
            if (!m_window)
            {
                printf("GLFW window creation failed!");
                glfwTerminate();
                return 1;
            }

            glfwGetFramebufferSize(m_window, &m_buffer_width, &m_buffer_height);
            glfwMakeContextCurrent(m_window);
            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK)
            {
                printf("GLEW initialisation failed!");
                glfwDestroyWindow(m_window);
                glfwTerminate();
                return 1;
            }

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, m_buffer_width, m_buffer_height);
            glfwSetWindowUserPointer(m_window, this);  


            return 0;
        }
        void destroy()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
        }
        void swap_buffers() { glfwSwapBuffers(m_window); }
        bool should_close() const { return glfwWindowShouldClose(m_window); }
        GLfloat get_buffer_width() const { return m_buffer_width; }
        GLfloat get_buffer_height() const { return m_buffer_height; }
        GLFWwindow* get_window() { return m_window; }
    private:
        GLFWwindow* m_window;
        GLint m_width{800};
        GLint m_height{600};
        GLint m_buffer_width;
        GLint m_buffer_height;
    };

} // namespace cwt
