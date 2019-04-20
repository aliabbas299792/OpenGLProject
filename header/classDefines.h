#ifndef CLASS_DEFINES
#define CLASS_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>

//assimp headers
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "structs.h"

class Shader{
    public:
        unsigned int ID;

        Shader(const char* v_file_path, const char* f_file_path);
		~Shader() {};
        
        void use(); //for using the shader in the program

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMatrix4(const std::string &name, glm::mat4 &matrix) const;
        void set3Float(const std::string &name, float r, float g, float b) const;
        void setVec3(const std::string &name, glm::vec3 &vector) const;
        void setVec3_v2(const std::string &name, glm::vec3 vector) const;
        void setVec4(const std::string &name, glm::vec4 &vector) const;

        /*
        When an object is defined as const in the main program, they can only call const member functions like those above.
        It is also considered good practice to make as many member functions const as possible to prevent accidental changes to objects.
        */
};

class Camera{
    private:
        int windowWidth = 0;
        int windowHeight = 0;

        float lastX = windowWidth/2;
        float lastY = windowHeight/2 + 250;
		float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch = 0;

	public:
        GLFWwindow* window = NULL;

        float fov = 45.0f;

        glm::mat4 projection = glm::mat4(1.0f);

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Camera(int width, int height, float fov);
		~Camera() {};
        void scroll_callback_zoom(double xOffset, double yOffset);
        void mouse_callback(double xPos, double yPos, bool &firstMouse);
        void liveUpdate();
};

class Mesh {
public:
	//variables for rendering
	unsigned int VAO, VBO, EBO;

	void setupMesh();

	//the structures holding the data for the mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//the mesh methods
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader* shader);
};

//the actual model, which may contain many meshes
class Model {
public:
	Model(std::string path);
	void Draw(Shader* shader);

	std::vector<Mesh> meshes;
	std::string directory;

	//so there would be a scene, and the scene would havve nodes, and the nodes may have meshes
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); //loaded materials stored here
	std::vector<Texture> textures_loaded; //loaded textures stored here
};

//the class that holds the player and all of their info
class Player {
private:
	Camera* camera = NULL;
	glm::vec3 dragForce = glm::vec3(0);

	glm::mat4 model = glm::mat4(1.0f);

	bool bounce = false;
	unsigned int maxBounce = 4;
	unsigned int currentBounces = 0;

	int firstFrame = 0;

	glm::vec2 acceleration = glm::vec2(0.0026, 0.08);
	glm::vec2 deceleration = glm::vec2(0.000108, 0.0004); //0.0004
	float originalDecelerationY = 0.0004; //0.0004
	float dragCoefficient = 0.16;

	glm::vec4 lastKeyStates = glm::vec4(0);

	float prevFrameTime = 0;
	float timeScalar = 0;

public:
	glm::vec3 pos = glm::vec3(0,0,0);
	glm::vec3 velocity = glm::vec3(0);
	bool onPlatform = false;

	Shader* shader = NULL;
	GLFWwindow* window = NULL;
	Model* player = NULL;

	Player(Camera* camera);
	~Player() {};
	void playerMovement(glm::vec4 keyStates);
	void liveUpdate(Shader* shader); //to update in the game loop
};

class Platforms {
private:

	glm::vec3 pos = glm::vec3(0.0f);
	glm::mat4 model = glm::mat4(1.0f);

	Player* player = NULL;
	Shader* shader = NULL;
	bool destroyed = false;
	int bounces = 0;

	float scale = 1;

public:
	static Model* platform;

	Platforms(Shader* shader, Player* player, glm::vec3 transform, float scale);
	~Platforms() {};
	void Draw();
	void liveUpdate();
	void collisionDetection();
	void transformPlatform(glm::vec3 transform);

	static bool onPlatform;
};

class chunks { //chunks, which will hold 9 platforms and each will cover the screen
public:
	glm::vec3 chunkCoords = glm::vec3(0); //these will be the global coordinates of the chunk
	glm::vec3 dimensions = glm::vec3(60);

	bool active = false;
	bool statusChanged = false;

	std::vector<Platforms> platformsInTheChunk; //a vector to hold the platforms

	chunks(int, int, int, Shader* shader, Player* player); //the constructor
	void chunkUpdate();
	~chunks(); //the destructor
};

class chunksHolder { //this will hold 9 chunks
private:
	Player* player = NULL;
	Shader* shader = NULL;

public:
	std::vector<chunks> chunksLoaded; //the vector to hold the chunks
	chunksHolder(glm::vec3 position, Shader* shader, Player* player); //the constructor
	~chunksHolder(); //the destructor

	glm::vec3 virtualChunk = glm::vec3(0); //this will be a virtual chunk, always relative to the position of the player

	void updateVirtualChunk();
	void virtualChunkHelper(glm::vec3 pos);
	void liveChunks();
};

#endif