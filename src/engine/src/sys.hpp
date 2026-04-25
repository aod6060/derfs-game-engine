#ifndef SYS_HPP
#define SYS_HPP

#include <cfloat>
// Once this file gets above 2000 to 3000 lines of code I'll refactor it.
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "vorbis/vorbisfile.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <deque>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>
#include <filesystem>

/*
#include <SDL.h>
#include <SDL_image.h>
*/

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <json/json.h>

#include <lua/lua.hpp>
#include <lua/lualib.hpp>
#include <lua/lauxlib.hpp>

#include <btBulletDynamicsCommon.h>

#include <AL/al.h>
#include <AL/alc.h>

#include "app/app.hpp"
#include "input/input.hpp"
#include "render/render.hpp"
#include "physics/physics.hpp"
#include "sound/sound.hpp"
#include "assets/assets.hpp"
#include "util/util.hpp"
#include "manager/manager.hpp"
#include "script/script.hpp"


#endif