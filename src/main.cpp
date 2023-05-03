
#include "pch.hpp"

#include "engine/engine.hpp"

class test_editor 
{
    public: 
        void render()
        {
            ImGui::Begin("my editor");

            ImGui::End();
        }
};
class test_editor2 
{
    public: 
        test_editor2(int value) : m_value(value){}
        
        void render()
        {
            ImGui::Begin("another editor");

            ImGui::End();
        }
    private:
        int m_value;
};




int main(int argc, char* argv[])
{
    cwt::engine engine;

    engine.attach_editor<test_editor>();
    engine.attach_editor<test_editor2>(14);

    while (engine.is_running())
    {
        engine.read_input();
        engine.update();
        engine.render();
    }
    
    return 0;
}