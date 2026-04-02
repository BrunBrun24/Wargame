#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



int main() {
    // Initialiser GLFW
    glfwInit();

    // Version OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//version de opengl (3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//version de opengl après le point (3.X)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Aucune idée pour le moment

    // Créer fenêtre 

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);  // Width height title IDK IDK 
    if (!window) {
        std::cout << "Erreur fenêtre\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Charger OpenGL avec GLAD
    // Glad etant Ce qui permet a opengl de fonctionner plus facilement.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Erreur GLAD\n";
        return -1;
    }

    //Il faut savoir que OPENGL utilise que des triangles
    float vertices[] = {
    -0.6f, -0.2f, 0.0f,
    -0.6f, +0.2f, 0.0f,
    +0.6f, -0.2f, 0.0f,

    -0.6f, +0.2f, 0.0f,
    +0.6f, +0.2f, 0.0f,
    +0.6f, -0.2f, 0.0f,
    
    +0.6f, +0.4f, 0.0f,
    +0.6f, +0.8f, 0.0f,
    -0.6f, +0.4f, 0.0f,

    -0.6f, +0.4f, 0.0f,
    -0.6f, +0.8f, 0.0f,
    +0.6f, +0.8f, 0.0f,

    +0.6f, -0.4f, 0.0f,
    +0.6f, -0.8f, 0.0f,
    -0.6f, -0.4f, 0.0f,

    -0.6f, -0.4f, 0.0f,
    -0.6f, -0.8f, 0.0f,
    +0.6f, -0.8f, 0.0f
    };//triangles des menus


    //Envoyer au GPU? 

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // dire comment lire les données
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


        const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
        )";


        const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";



unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

unsigned int shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);




    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f); //mets en bleu, après faut comprendre comment sa fonctionne
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}