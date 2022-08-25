#include "BindingPass.h"
#include "RenderGraph/Bindable/Bindable.hpp"

namespace rg {

BindingPass::BindingPass(const std::string &passName)
: Pass(passName)
, pRenderTarget(this, "RenderTarget")
, pDepthStencil(this, "DepthStencil")
{
}

void BindingPass::link(dx12lib::ICommonContext &commonCtx) const {
	pRenderTarget.link(commonCtx);
	pDepthStencil.link(commonCtx);
}

void BindingPass::addBind(std::shared_ptr<Bindable> pBindable) {
	_bindables.push_back(std::move(pBindable));
}

void BindingPass::bindAll(dx12lib::IGraphicsContext &graphicsCtx) const {
	link(graphicsCtx);
	for (auto &pBindable : _bindables)
		pBindable->bind(graphicsCtx);
}

void BindingPass::bindRenderTarget(dx12lib::IGraphicsContext &graphicsCtx) const {
	if (pRenderTarget != nullptr) {
		graphicsCtx.setRenderTarget(
			pRenderTarget->getRTV(renderTargetMipmap),
			pDepthStencil->getDSV()
		);
	} else {
		graphicsCtx.setRenderTarget(pDepthStencil->getDSV());
	}
}

DXGI_FORMAT BindingPass::getRtFormat(size_t slot) const {
	return pRenderTarget->getFormat();
}

DXGI_FORMAT BindingPass::getDsFormat() const {
	return pDepthStencil->getFormat();
}

void BindingPass::reset() {
	Pass::reset();
	pRenderTarget.reset();
	pDepthStencil.reset();
}

}
