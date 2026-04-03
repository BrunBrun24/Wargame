# Explication détaillée du code OpenGL

## 1. Définition des vertices (points)

float vertices[] = {
-0.5f, -0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
0.5f,  0.5f, 0.0f,

```
 0.5f,  0.5f, 0.0f,
-0.5f,  0.5f, 0.0f,
-0.5f, -0.5f, 0.0f
```

};

Ce tableau contient les coordonnées des points du carré.

Chaque groupe de 3 valeurs = 1 point :
(x, y, z)

Tu dessines un carré avec 2 triangles :

* triangle 1 : points 1, 2, 3
* triangle 2 : points 4, 5, 6

OpenGL ne comprend que les triangles → donc même un carré = 2 triangles.

---

## 2. Création des buffers (VBO + VAO)

unsigned int VBO, VAO;

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

* VBO (Vertex Buffer Object) : stocke les données (les vertices)
* VAO (Vertex Array Object) : décrit comment lire ces données

---

## 3. Binding (connexion)

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

* glBindVertexArray : active le VAO
* glBindBuffer : active le VBO
* glBufferData : envoie les données CPU → GPU

GL_STATIC_DRAW = les données ne changent pas souvent

---

## 4. Configuration des attributs

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

Explication :

* 0 = location dans le shader
* 3 = 3 valeurs par sommet (x, y, z)
* GL_FLOAT = type float
* stride = taille d’un vertex (3 floats)
* offset = 0 (on commence au début)

glEnableVertexAttribArray(0) active cet attribut

---

## 5. Vertex Shader

const char* vertexShaderSource = ...

Rôle :

* transforme les positions
* obligatoire en OpenGL moderne

gl_Position = position finale à l’écran

---

## 6. Fragment Shader

const char* fragmentShaderSource = ...

Rôle :

* définit la couleur de chaque pixel

ici :
FragColor = orange (1.0, 0.5, 0.2)

---

## 7. Compilation des shaders

glCreateShader → crée shader
glShaderSource → donne le code
glCompileShader → compile

Puis :

glCreateProgram → programme GPU
glAttachShader → attache vertex + fragment
glLinkProgram → lie tout

glDeleteShader → cleanup (plus besoin après)

---

## 8. Boucle principale

while (!glfwWindowShouldClose(window)) {

Boucle infinie jusqu’à fermeture

---

## 9. Nettoyage écran

glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);

* définit la couleur de fond
* efface l’écran

---

## 10. Dessin

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 6);

* active le shader
* active les données
* dessine

GL_TRIANGLES → interprète les points comme triangles
0 → début
6 → nombre de vertices

---

## 11. Mise à jour fenêtre

glfwSwapBuffers(window);
glfwPollEvents();

* swap buffers = affiche l’image
* poll events = clavier/souris

---

## Résumé global

Pipeline OpenGL :

1. Définir les points
2. Envoyer au GPU
3. Configurer lecture
4. Compiler shaders
5. Boucle :

   * clear
   * draw
   * display

---

## Point important à retenir

OpenGL ne dessine rien "directement"

Tu dois :

* décrire des données
* dire comment les lire
* utiliser un shader

C’est pour ça que c’est verbeux au début

---

## Ce que tu peux faire ensuite

* Ajouter des couleurs par vertex
* Faire bouger la forme
* Ajouter une caméra
* Charger des textures

---
