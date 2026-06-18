#pragma once
#include "Mesh.h"
#include "Component.h"
#include <unordered_map>

class MeshRenderer : public Component {
    std::vector<Mesh> meshes;

    static std::unordered_map<std::string, std::vector<Mesh>>& get_cache() {
        static std::unordered_map<std::string, std::vector<Mesh>> cache;
        return cache;
    }
public:
    MeshRenderer(const std::string& path) {
        auto& cache = get_cache();
        auto it = cache.find(path);
        if (it != cache.end()) {
            meshes = it->second;
        }
        else {
            meshes = load_meshes(path);
            cache[path] = meshes;
        }
    }

    void init() override {};

    void render() override {
        for (const auto& mesh : meshes) {

            mesh.material.apply();
            mesh.material.handle_transparency();

            glEnable(GL_LIGHTING);

            if (!mesh.material.diffuse_map) {
                glColor4fv(mesh.material.diffuse);
            }

            glBegin(GL_TRIANGLES);
            for (const auto& v : mesh.vertices) {
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                Material::glMultiTexCoord2fARB(GL_TEXTURE0, v.texcoord.x, v.texcoord.y);
                Material::glMultiTexCoord2fARB(GL_TEXTURE1, v.texcoord.x, v.texcoord.y);
                glVertex3f(v.position.x, v.position.y, v.position.z);
            }

            glEnd();
            mesh.material.cleanup();
        }
    }
};