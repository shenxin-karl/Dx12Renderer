#pragma once
#include <vector>
#include <memory>
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/Context/ContextProxy.hpp>

namespace rgph {

class Material;
class Step {
public:
	explicit Step(Material *pMaterial, std::string subPassName);
	void addBindable(std::shared_ptr<Bindable> pBindable);
	void submit(const Geometry *pGeometry, const TransformCBuffer *pTransformCBuffer) const;
	void bind(dx12lib::IGraphicsContext &graphicsCtx) const;
private:
	SubPass *_pSubPass;
	Material *_pMaterial;
	std::string _subPassName;
	std::vector<std::shared_ptr<Bindable>> _bindables;
};

}
