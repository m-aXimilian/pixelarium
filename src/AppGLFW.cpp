#include "AppGLFW.hpp"

#include <memory>

#include "imaging/Image.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "portable-file-dialogs.h"
#include "rendering/CvMatRender.hpp"
#include "uiresources.h"

using namespace pixelarium::imaging;

/*static*/ bool pixelarium::ui::dim_changed_p(const ImVec2& ref_rect, const ImVec2& new_rect)
{
    if (std::abs(ref_rect.y - new_rect.y) > 5 || std::abs(ref_rect.x - new_rect.x))
    {
        return true;
    }

    return false;
}

/*static*/ ImVec2 pixelarium::ui::ascpet_const_dimensions(const pixelarium::imaging::Image& img, const ImVec2& curr_dim)
{
    const auto w_fact = (static_cast<float>(curr_dim.x) / img.GetImage().cols);
    const auto h_fact = (static_cast<float>(curr_dim.y) / img.GetImage().rows);

    const auto fact = w_fact < h_fact ? w_fact : h_fact;

    return ImVec2(img.GetImage().cols * fact, img.GetImage().rows * fact);
}

void pixelarium::ui::AppGLFW::InitMainWindow()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
#ifdef __linux__
    const char* glsl_version = "#version 130";
#else
    const char* glsl_version = reinterpret_cast<const char*>("#version 130");
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // int count;
    // GLFWmonitor** monitors = glfwGetMonitors(&count); // at [0] is always the
    // main monitor GLFWmonitor* monitor = monitors[1];
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);

    // lg::Logger::Info("screen width " + std::to_string(width) +
    //                  " screen heigth " + std::to_string(height));

    // Create window with graphics context
    window = glfwCreateWindow(1200, 800, PIXELARIUM_TITLE, nullptr, nullptr);
    if (window == nullptr)
    {
        // lg::Logger::Error("no window");
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform
                                                           // Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

int pixelarium::ui::AppGLFW::Run()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    while (!glfwWindowShouldClose(this->window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetID("Backspace"));

        this->MenuBar();

        if (this->_imagep)
        {
            // auto render = render::CvMatRender(this->_img);
            ImGui::Begin("An image", &this->_imagep, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            this->_curr_dim = ImGui::GetContentRegionAvail();
            auto new_dim = ImGui::GetContentRegionAvail();
            auto texture = dim_changed_p(this->_curr_dim, new_dim)
                               ? this->_render.Render(static_cast<size_t>(this->_curr_dim.x),
                                                      static_cast<size_t>(this->_curr_dim.y))
                               : this->_render.Render();

            this->_curr_dim = new_dim;

            // random aspect ratio
            // ImGui::Image(reinterpret_cast<Textured>(
            //                  reinterpret_cast<void*>(*texture)),
            //              this->_curr_dim);
            ImVec2 dim(this->_img->GetImage().cols, this->_img->GetImage().rows);
            // aspect ratio constant render
            ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(*texture)),
                         ascpet_const_dimensions(*this->_img, new_dim));

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(this->window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(this->window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();

    return 0;
}

void pixelarium::ui::AppGLFW::MenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        // main menu
        if (ImGui::BeginMenu(MAINMENUNAME))
        {
            ImGui::EndMenu();
        }

        // file menu
        if (ImGui::BeginMenu(FILEMENUNAME))
        {
            if (ImGui::MenuItem("Load File"))
            {
                this->LoadImageProt();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void pixelarium::ui::AppGLFW::LoadImageProt()
{
    auto res{pfd::open_file("Load Inputs", pfd::path::home(), {"All Files", "*"}, pfd::opt::multiselect).result()};
    for (auto& p : res)
    {
        // lg::Logger::Debug("Adding image from " + std::string(p),
        // __FUNCTION__);
        if (this->_logger)
        {
            this->_logger->Warn("Creating image");
        }
        // this->_img = Image(p);
        this->_img = std::make_shared<Image>(p);
        this->_render = pixelarium::render::CvMatRender(this->_img);
        this->_imagep = true;
    }
}