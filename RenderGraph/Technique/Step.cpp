#include "Step.h"
#include "RenderGraph/Bindable/Bindable.hpp"
#include "RenderGraph/Drawable/Drawable.h"
#include "RenderGraph/Job/Job.h"
#include "RenderGraph/Pass/RenderQueuePass.h"

namespace rgph {

Step::Step(Material *pMaterial, std::string subPassName)
: _pSubPass(nullptr), _pMaterial(pMaterial)
, _subPassName(std::move(subPassName))
{
}

void Step::addBindable(std::shared_ptr<Bindable> pBindable) {
	assert(pBindable != nullptr);
	_bindables.push_back(std::move(pBindable));
}

void Step::submit(const Geometry *pGeometry, const TransformCBuffer *pTransformCBuffer) const {
	assert(_pSubPass != nullptr);
	_pSubPass->accept({ this, pGeometry, pTransformCBuffer });
}

void Step::bind(dx12lib::IGraphicsContext &graphicsCtx) const {
	for (auto &pBindable : _bindables)
		pBindable->bind(graphicsCtx);
}

}
