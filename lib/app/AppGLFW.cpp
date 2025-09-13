#include "AppGLFW.hpp"

#include "app_resources_default.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/// @brief GLFW error callback function.
/// @param error The error code.
/// @param description A description of the error.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

/// @brief Initializes the main application window using GLFW.
///
/// This function sets up the GLFW error callback, initializes GLFW,
/// selects appropriate OpenGL and GLSL versions based on platform,
/// creates the main application window, sets up the OpenGL context,
/// enables vsync, and initializes the Dear ImGui context, style, and
/// platform/renderer backends.  It uses the primary monitor's workarea
/// to determine the screen resolution but creates a window of a fixed size (1200x800).
///
/// @return void.  The function returns void and uses error handling internally to manage GLFW and window creation
/// failures.  The application may continue to run in a failed state, but the window will not be initialized.
void pixelarium::application::AppGLFW::InitMainWindow()
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

/// @brief Main application loop.  This function handles window events, rendering, and ImGui integration.
/// @return 0 if the application closes successfully.
int pixelarium::application::AppGLFW::RunLoop()
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

        this->Run();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(this->window, &display_w, &display_h);
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

/// @brief Creates and displays the main menu bar.
///
/// This function constructs the application's main menu bar using ImGui.  It includes a log level selector
/// and calls other functions to populate additional menu bar sections.
void pixelarium::application::AppGLFW::MenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        // main menu
        if (ImGui::BeginMenu(MAINMENUNAME))
        {
            if (ImGui::BeginCombo(LOGLEVELSELECT, LOGLEVELS[log_level_].data()))
            {
                for (int n = 0; n < static_cast<int>(LOGLEVELS.size()); n++)
                {
                    bool is_selected = (LOGLEVELS[log_level_] == LOGLEVELS[n]);
                    if (ImGui::Selectable(LOGLEVELS[n].data(), is_selected))
                    {
                        log_level_ = n;
                        this->logger_.ChangeLevel(static_cast<utils::log::LogLevel>(1 << log_level_));
                    }
                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // consumer main menu bar entries
            this->MenuBarOptionsColumn1();

            ImGui::EndMenu();
        }

        // consumer menu bar columns
        this->MenuBarOptionsColumn2();
        this->MenuBarOptionsColumn3();
        this->MenuBarOptionsColumn4();
        this->MenuBarOptionsColumn5();

        ImGui::EndMainMenuBar();
    }
}

/// @brief Allows the user to select the log level via a combo box.
void pixelarium::application::AppGLFW::LogLevelSelect()
{
    if (ImGui::BeginCombo(LOGLEVELSELECT, LOGLEVELS[log_level_].data()))
    {
        for (int n = 0; n < static_cast<int>(LOGLEVELS.size()); n++)
        {
            bool is_selected = (LOGLEVELS[log_level_] == LOGLEVELS[n]);
            if (ImGui::Selectable(LOGLEVELS[n].data(), is_selected))
            {
                log_level_ = n;
                this->logger_.ChangeLevel(static_cast<utils::log::LogLevel>(1 << log_level_));
            }
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
