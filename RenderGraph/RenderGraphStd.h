#pragma once
#include <Dx12lib/Context/CommonContext.h>
#include <Math/MathStd.hpp>

namespace rgph {

using namespace Math;
struct TechniqueFlag;
class Technique;
class Drawable;
class Bindable;
class Step;
class SubPass;
class RenderQueuePass;
class Job;
class GraphicsPSOBindable;
struct TechniqueType;
struct DrawArgs;
class Geometry;
class TransformCBuffer;

class NonCopyable {
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable &) = delete;
	NonCopyable &operator=(const NonCopyable &) = delete;
	virtual ~NonCopyable() = default;
};

struct IDrawable : public NonCopyable {
	IDrawable() = default;
	~IDrawable() override = default;
	virtual void bind(dx12lib::IGraphicsContext &graphicsCtx) const = 0;
	virtual void submit(const TechniqueFlag &techniqueFlag) const = 0;
	virtual bool addTechnique(std::shared_ptr<Technique> pTechnique) = 0;
	virtual bool hasTechnique(const TechniqueType &techniqueType) const = 0;
	virtual bool removeTechnique(const TechniqueType &techniqueType) = 0;
	virtual void setTechniqueActive(const TechniqueType &techniqueType, bool bActivate) = 0;
};

}
