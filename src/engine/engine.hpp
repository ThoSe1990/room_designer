#pragma once 

#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"

#include "framebuffer.hpp"

namespace cwt
{

    class test_editor 
    {
        public: 
            void render()
            {
                ImGui::Begin("any editor");

                ImGui::End();

                ImGui::Begin("anotherr");

                ImGui::End();
            }
    };


    class object_3d 
    {
        public:
            object_3d(glm::vec3 translation, glm::vec3 scale) : m_translation(translation), m_scale(scale) 
            {
                m_mesh_list.push_back(create_mesh());
            }
            void render(glm::mat4 projection, GLuint uniform_model, GLuint uniform_projection)
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, m_translation);
                model = glm::scale(model, m_scale);
                glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
                for (const auto& mesh : m_mesh_list) {
                    mesh->render();
                }
            }
        private:
            glm::vec3 m_translation;
            glm::vec3 m_scale;
            std::vector<std::unique_ptr<mesh>> m_mesh_list;
    };

    class scene 
    {
        public: 
            scene(const std::string& name, GLfloat width, GLfloat height) : m_name(name)
            {
                m_projection = glm::perspective(45.0f, width/height, 0.1f, 100.0f);
                m_scenebuffer = std::make_shared<framebuffer>(width, height);
            }
            void add_object()
            {
                m_objects.push_back(std::make_unique<object_3d>(glm::vec3(0.0f, 0.0f, -2.5f), glm::vec3(0.4f, 0.4f, 1.0f)));
            }
            void render(GLuint uniform_model, GLuint uniform_projection)
            {
                m_scenebuffer->Bind();
                
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);
                

                ImGui::Begin(m_name.c_str());

                ImVec2 vMin = ImGui::GetWindowContentRegionMin();
                ImVec2 vMax = ImGui::GetWindowContentRegionMax();

                vMin.x += ImGui::GetWindowPos().x;
                vMin.y += ImGui::GetWindowPos().y;
                vMax.x += ImGui::GetWindowPos().x;
                vMax.y += ImGui::GetWindowPos().y;
                
                m_scenebuffer->RescaleFrameBuffer(vMax.x-vMin.x, vMax.y-vMin.y);
                glViewport(0, 0, vMax.x-vMin.x, vMax.y-vMin.y);

                ImGui::GetWindowDrawList()->AddImage(
                    (void *)m_scenebuffer->getFrameTexture(),
                    vMin, vMax,
                    ImVec2(0, 1), 
                    ImVec2(1, 0)
                );

                ImGui::End();

                for (const auto& obj : m_objects) {
                    obj->render(m_projection, uniform_model, uniform_projection);
                }

                m_scenebuffer->Unbind();
            }

        private:
            std::string m_name;
            glm::mat4 m_projection;
            std::shared_ptr<framebuffer> m_scenebuffer;
            std::vector<std::unique_ptr<object_3d>> m_objects;
    };


    class editor 
    {
        public:
            template<typename ConcreteEditor> 
            editor(ConcreteEditor&& concrete_editor) 
            {
                m_editor = std::make_unique<editor_model<ConcreteEditor>>(std::move(concrete_editor));
            }

            void render() const
            {
                m_editor->render();
            }
        private:
            struct editor_concept{
                virtual ~editor_concept() {}
                virtual void render() = 0;
            };
            template<typename ConcreteEditor> 
            struct editor_model : public editor_concept
            {
                editor_model(const ConcreteEditor& e) : m_editor(e) {}
                void render() {
                    m_editor.render();
                }
                ConcreteEditor m_editor;
            };

        private:
            std::unique_ptr<editor_concept> m_editor;
    };

    class imgui_manager
    {
        public:
            imgui_manager() = default;
            ~imgui_manager()
            {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
            }
            void init(GLFWwindow* window)
            {
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 

                ImGui::StyleColorsDark();
                ImGuiStyle& style = ImGui::GetStyle();
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    style.WindowRounding = 0.0f;
                    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
                }
                ImGui_ImplGlfw_InitForOpenGL(window, true);
                ImGui_ImplOpenGL3_Init("#version 330");
            }
            void new_frame()
            {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();  
                ImGui::NewFrame();
            }

            void render()
            {
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }
            }  
    };


    class engine 
    {
        public:
            engine();
            ~engine();
            bool is_running();
            void read_input();
            void update();
            void render();

        private:
            void render_editors() 
            {
                for (const auto& e : m_editors){
                    e->render();
                }
            }
            void render_scenes(GLuint uniform_model, GLuint uniform_projection)
            {
                for (const auto& s : m_scenes) {
                    s->render(uniform_model, uniform_projection);
                }
            }
        private:
            window m_window; 
            imgui_manager m_imgui;
            std::vector<std::unique_ptr<shader>> m_shader_list;
            std::vector<std::unique_ptr<editor>> m_editors;
            std::vector<std::unique_ptr<scene>> m_scenes;
    };


    engine::engine() 
    {
        m_window.init();
        m_imgui.init(m_window.get_window());
        
        m_editors.push_back(std::make_unique<editor>(test_editor{}));

        m_scenes.push_back(std::make_unique<scene>("scene 1", static_cast<GLfloat>(800.0f), static_cast<GLfloat>(600.0f)));
        m_scenes.back()->add_object();
        m_scenes.push_back(std::make_unique<scene>("scene 2", static_cast<GLfloat>(800.0f), static_cast<GLfloat>(600.0f)));
        m_scenes.back()->add_object();

        m_shader_list.push_back(std::make_unique<shader>(vertex_shader_file, fragment_shader_file));
    }
    engine::~engine()
    {
        m_window.destroy();
    }
    bool engine::is_running()
    {
        return !m_window.should_close();
    }
    void engine::read_input()
    {
        glfwPollEvents();
    }
    void engine::update()
    {

    }
    void engine::render()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        m_imgui.new_frame();
        
        m_shader_list[0]->use();
        render_scenes(m_shader_list[0]->get_model_location(), m_shader_list[0]->get_projection_location());  
        render_editors();  

        m_imgui.render();
        m_window.swap_buffers();
    }
} // namespace cwt
