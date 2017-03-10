#ifndef TRAILRENDERER_h
#define TRAILRENDERER_h

#include "rendering.hpp"
#include <Model/GameObject.h>

struct TrailMaterialTraits {
	static constexpr const char* ShaderProgramName = "trail_renderer_shader_prog";
	static constexpr const char* VertexShaderPath = "shaders\\Vertex.glsl";
	static constexpr const char* FragmentShaderPath = "shaders\\Fragment.glsl";
	typedef float PerVertexData;
	typedef std::vector<PerVertexData> MeshData;
	typedef Mesh<PerVertexData> Mesh;
};

class TrailRenderer : public Renderer<TrailRenderer, TrailMaterialTraits>
{
	friend class Renderer<TrailRenderer, TrailMaterialTraits>;
	GameObject* gameObject;
public:
	explicit TrailRenderer(GameObject* gameObject);
	virtual ~TrailRenderer() override;
	virtual void render() const override;
};

#endif //TRAILRENDERER_h