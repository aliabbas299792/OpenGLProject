#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../header/classDefines.h"
#include <glm/ext.hpp>

chunksHolder::~chunksHolder() {
	for (int i = 0; i < chunksLoaded.size(); i++) {
			chunksLoaded[i].~chunks();
			chunksLoaded.pop_back();
	}
}

chunksHolder::chunksHolder(glm::vec3 pos, Shader* shader, Player* player) {
	this->player = player;
	this->shader = shader;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				chunksLoaded.push_back(
					chunks(
						pos.x + 100 * i,
						pos.y + 100 * j,
						pos.z + 100 * k,
						shader,
						player
					)
				);
			}
		}
	}
}

void chunksHolder::liveChunks() {
	for (int i = 0; i < chunksLoaded.size(); i++) {
		chunksLoaded[i].chunkUpdate();
	}
	updateVirtualChunk();
}

void chunksHolder::updateVirtualChunk() {
	virtualChunk.x = floor((player->pos.x) / 100) * 100;
	virtualChunk.y = floor((player->pos.y) / 100) * 100;
	virtualChunk.z = floor((player->pos.z) / 100) * 100;

	//I didn't use a loop here, because this makes it easier to visualise
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y - 100, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 100, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y - 100, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y + 100, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 100, virtualChunk.z - 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y + 100, virtualChunk.z - 100));

	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y - 100, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 100, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y - 100, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y + 100, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 100, virtualChunk.z));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y + 100, virtualChunk.z));

	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y - 100, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y - 100, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y - 100, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x - 100, virtualChunk.y + 100, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x, virtualChunk.y + 100, virtualChunk.z + 100));
	virtualChunkHelper(glm::vec3(virtualChunk.x + 100, virtualChunk.y + 100, virtualChunk.z + 100));
	//assuming -100k is back, 0k is center, 100k is front (from i,j,k)
	
	for (int i = 0; i < chunksLoaded.size(); i++) {
		if (chunksLoaded[i].chunkCoords.x < virtualChunk.x - 100 || chunksLoaded[i].chunkCoords.x > virtualChunk.x + 100 ||
			chunksLoaded[i].chunkCoords.y < virtualChunk.y - 100 || chunksLoaded[i].chunkCoords.y > virtualChunk.y + 100 ||
			chunksLoaded[i].chunkCoords.z < virtualChunk.z - 100 || chunksLoaded[i].chunkCoords.z > virtualChunk.z + 100
			) {
			chunksLoaded[i].~chunks();
			chunksLoaded.erase(chunksLoaded.begin() + i);
		}
	}
}

void chunksHolder::virtualChunkHelper(glm::vec3 pos) {
	//std::cout << glm::to_string(pos) << std::endl;
	bool found = false;
	for (int i = 0; i < chunksLoaded.size(); i++) {
		if (chunksLoaded[i].chunkCoords == pos) {
			found = true;
			break;
		}
	}

	if (found == false) {
		chunksLoaded.push_back(
			chunks(
				pos.x,
				pos.y,
				pos.z,
				shader,
				player
			)
		);
	}
}