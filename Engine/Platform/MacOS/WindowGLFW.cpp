#include "Platform/MacOS/WindowGLFW.h"
#include "Events/EventManager.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

WindowGLFW::WindowGLFW()
{
}


void WindowGLFW::Init(const std::string& title, bool is_fullscreen, u32 width, u32 height)
{
    width_ = width;
    height_ = height;
    isFullscreen_ = is_fullscreen;

    // Initialize and configure glfw window 
    if (!glfwInit())
        exit(EXIT_FAILURE);

#ifdef GRAPHIC_API_OPENGL
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef GRAPHIC_API_VULKAN
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

    // Create GLFW window
    monitor_ = glfwGetPrimaryMonitor();
    if (is_fullscreen) 
    {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
        width_ = mode->width;
        height_ = mode->height;
    }

    window_ = glfwCreateWindow(width_, height_, title.c_str(), is_fullscreen? monitor_ : nullptr, nullptr);
    if (!window_) {
        std::cerr << "Cannot create GLFW window.\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
#ifdef GRAPHIC_API_OPENGL
    glfwMakeContextCurrent(window_);
#endif

    glfwSetWindowUserPointer(window_, reinterpret_cast<void*>(this));

#ifdef GRAPHIC_API_OPENGL
    // Initialize glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) 
    {
        auto owner = *(WindowGLFW*)glfwGetWindowUserPointer(window);
        owner.width_ = width;
        owner.height_ = height;
        EventManager::Instance().ImmediateTrigger(WindowResizeEvent(width, height));
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        std::unique_ptr<WindowCloseEvent> closeEvent = std::make_unique<WindowCloseEvent>();
        EventManager::Instance().QueueEvent(std::move(closeEvent));
    });
    
    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        switch (action) {
        case GLFW_PRESS: {
            EventManager::Instance().ImmediateTrigger(KeyPressedEvent(key, 0));
            break;
        }
        case GLFW_RELEASE: {
            EventManager::Instance().ImmediateTrigger(KeyReleasedEvent(key));
            break;
        }
        case GLFW_REPEAT: {
            EventManager::Instance().ImmediateTrigger(KeyPressedEvent(key, 1));
            break;
        }
        }
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos)
    {
        EventManager::Instance().ImmediateTrigger(MouseMovedEvent((float)xpos, (float)ypos));
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset) {
        EventManager::Instance().ImmediateTrigger(MouseScrolledEvent((float)xOffset, (float)yOffset));
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos) {
        EventManager::Instance().ImmediateTrigger(MouseMovedEvent((float)xPos, (float)yPos));
    });

#ifdef GRAPHIC_API_OPENGL
    // Configure global OpenGl state
    glEnable(GL_DEPTH_TEST);
#endif
}
void WindowGLFW::Finalize()
{
    glfwTerminate();
}

void WindowGLFW::Update()
{
    // swap buffers
    glfwSwapBuffers(window_);
    // handle events
    glfwPollEvents();
}