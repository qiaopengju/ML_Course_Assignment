#include <mutex>
#include "render.hpp"
#include "game.hpp"

using namespace std;

GLFWwindow* window;
bool flag_mouse_click = false;  //鼠标按下触发器
const unsigned SCR_WIDTH = 800;
const unsigned SCR_HEIGHT = 800;
// vertice data
float vertices[] = { // 三角形的标准化设备坐标(Normalized device coordinates), [-1, 1]
    //-----position--------|-texture--|
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   //top right
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //bottom right 
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   //bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f    //top left
};
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void mouse2pos(float x, float y){
    if (x <= 40.0f || x >= 760.0f || y <= 40 || y >= 760){
        action = "";
        return;
    }
    int i = (x - 40) / 90;
    int j = (y - 40) / 90;
    action = "";
    position2s(action, j, i);
}

void render(const Game &status){
    init_GL();

    ResourceManager::LoadShader("Shaders/2Dtexture.vert", "Shaders/2DTexture.frag", "shader");
    ResourceManager::LoadTexture2D("src/board.jpeg", "board");
    ResourceManager::LoadTexture2D("src/black.png", "black");
    ResourceManager::LoadTexture2D("src/white.png", "white");
    ResourceManager::LoadTexture2D("src/win.png", "win");
    ResourceManager::LoadTexture2D("src/lose.png", "lose");
    ResourceManager::LoadTexture2D("src/a_draw.png", "a_draw");
    ResourceManager::LoadTexture2D("src/valid.png", "valid");
    ResourceManager::getShader("shader").setInt("texture1", 0);

    int ID = ResourceManager::getShader("shader").ID;
    unsigned int VBO, VAO, EBO;
    /* set up vertex data and buffers and configure vertex attributes */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    /* bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attrtibutes */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* 设置顶点position属性 */
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    /* 设置顶点texture坐标 */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    /* Render Loop 渲染循环 */
    /*---------------------*/
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        ResourceManager::getShader("shader").use();

        glm::mat4 model(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        ResourceManager::getTexture2D("board").bind();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //遍历黑子
        for (int i = 0; i < status.board.black.size(); i++){
            float x, y; get_position(status.board.black[i], y, x); //****
            float x_trans(0), y_trans(0);
            y = 7 - y;
            x_trans = -0.7875 + x*0.225;
            y_trans = -0.7875 + y*0.225;

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x_trans, y_trans, 1.0));
            model = glm::scale(model, glm::vec3(0.11));
            glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            ResourceManager::getTexture2D("black").bind();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        //遍历白子
        for (int i = 0; i < status.board.white.size(); i++){
            float x, y; get_position(status.board.white[i], y, x); //****
            float x_trans(0), y_trans(0);
            y = 7 - y;
            x_trans = -0.7875 + x*0.225;
            y_trans = -0.7875 + y*0.225;

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x_trans, y_trans, 1.0));
            model = glm::scale(model, glm::vec3(0.11));
            glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            ResourceManager::getTexture2D("white").bind();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        //遍历有效位置
        if (status.board.turn == BLACK_TURN){
            mutex_valid_pos.lock();
            auto tmp = status.board.valid_position;
            mutex_valid_pos.unlock();
            for(auto it = tmp.begin(); it != tmp.end(); it++){
                assert(tmp.size() != 0);
                assert(it != tmp.end());
                assert(it->first.size() == 2);
                float x, y; get_position(it->first, y, x); 
                float x_trans(0), y_trans(0);
                y = 7 - y;
                x_trans = -0.7875 + x*0.225;
                y_trans = -0.7875 + y*0.225;

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x_trans, y_trans, 1.0));
                model = glm::scale(model, glm::vec3(0.11));
                glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ResourceManager::getTexture2D("valid").bind();
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        if (status.is_terminal()){
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            if (status.board.score > 0){
                ResourceManager::getTexture2D("win").bind();
            } else if (status.board.score < 0){
                ResourceManager::getTexture2D("lose").bind();
            } else{
                ResourceManager::getTexture2D("a_draw").bind();
            }
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window); // 交换前后缓冲
        glfwPollEvents(); // poll events 回调事件
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){ // 光标移动回调
    //cout << "X:" << xpos << "\t Y:" << ypos << "\n";
    if (flag_mouse_click){
        flag_mouse_click = false;
        mouse2pos(xpos, ypos);
        //cout << "POS " << action << "\n";
    }
}
void mouse_button_callback(GLFWwindow *window, int button, int act, int mods){   //鼠标按下回调
    if (button == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS){
        flag_mouse_click = true;
    }
}

void init_GL(){
    /* glfw: initalize and cofigure */
    glfwInit(); //init
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //major version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //minor version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X
#endif

    /* glfw window creation */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Reversi", NULL, NULL); //create a window
    if (window == NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        assert(0);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //注册窗口大小改变回调函数
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    /* glad: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //GLAD
        cout << "Failed to initialize GLAD" << endl;
        assert(0);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ // 窗口大小改变时回调该函数
    glViewport(0, 0, width, height);
}
