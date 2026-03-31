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

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}