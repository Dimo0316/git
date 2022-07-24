#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                "   gl_PointSize = 50.0f;\n"
                                "}\0";

// 片段着色器源码
const char *fragmentShaderSource = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "   FragColor = vec4(1.0f, 1.0f, 0.7f, 1.0f);\n"
                                "}\0";
// 窗口渲染回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 从窗口输入 ESC 退出窗口
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 设置视图
    glViewport(0, 0, 800, 600);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // 注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义顶点数组
    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f,   // 右上角
    //     0.5f, -0.5f, 0.0f,  // 右下角
    //     -0.5f, -0.5f, 0.0f, // 左下角
    //     -0.5f, 0.5f, 0.0f   // 左上角
    // };
    float vertices[] = {
        // 第一个三角形
        -1.0f, 0.0f, 0.0f,
        0.0f,  0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        // 第二个三角形
        0.0f,  0.0f, 0.0f,
        1.0f,  0.0f, 0.0f,
        0.5f,  0.5f, 0.0f
    };

    unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // 创建顶点缓冲对象
    unsigned int VBO[2], VAO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);

    // 绑定VAO1对象
    glBindVertexArray(VAO[0]);

    // 绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    // 填充缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 告诉 OpenGL 如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


     // 绑定VAO1对象
    glBindVertexArray(VAO[1]);

    // 绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    // 填充缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 告诉 OpenGL 如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) (9*sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // 已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理。
    // 下面我们会创建一个顶点着色器和片段着色器来真正处理这些数据。

    // 创建顶点着色器和片段着色器对象
    unsigned int vertexShader, fragmentShader1, fragmentShader2;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader1);
    glCompileShader(fragmentShader2);

    // 检测是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 着色器程序，链接着色器
    // 创建程序对象
    unsigned int shaderProgram1, shaderProgram2;
    shaderProgram1 = glCreateProgram();
    shaderProgram2 = glCreateProgram();
    // 将着色器连接到程序上
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    // 链接
    glLinkProgram(shaderProgram1);
    glLinkProgram(shaderProgram2);

    // 检测链接是否成功
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAMGE1::LINK_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAMGE2::LINK_FAILED\n" << infoLog << std::endl;
    }

    // 使用着色器程序
    // glUseProgram(shaderProgram1);
    // glUseProgram(shaderProgram2);

    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.2f, 0.8f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        // 检查并调用时间，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}