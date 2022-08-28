#include "ExecutablePass.h"
#include "RenderGraph/Bindable/Bindable.hpp"

namespace rgph {

ExecutablePass::ExecutablePass(const std::string &passName, bool rtActivate, bool dsActivate)
: Pass(passName), _passResources({})
, pRenderTarget(this, "RenderTarget", rtActivate)
, pDepthStencil(this, "DepthStencil", dsActivate)
{
}

PassResourceBase * ExecutablePass::getPassResource(const std::string &resourceName) const {
	for (auto *pPassResource : _passResources) {
		if (pPassResource->isActivated() && pPassResource->getResourceName() == resourceName)
			return pPassResource;
	}
	return nullptr;
}

void ExecutablePass::preExecute(dx12lib::DirectContextProxy pDirectCtx) {
	Pass::preExecute(pDirectCtx);
	for (auto *pPassResource : _passResources) {
		if (pPassResource->isActivated())
			pPassResource->link(*pDirectCtx);
	}
}

void ExecutablePass::execute(dx12lib::DirectContextProxy pDirectCtx) {
	bindRenderTarget(*pDirectCtx);
}

void ExecutablePass::postExecute(dx12lib::DirectContextProxy pDirectCtx) {
	Pass::postExecute(pDirectCtx);
	for (auto *pPassResource : _passResources) {
		if (pPassResource->isActivated())
			pPassResource->setFinished(true);
	}
}

void ExecutablePass::bindRenderTarget(dx12lib::IGraphicsContext &graphicsCtx) {
	size_t width = pDepthStencil->getWidth();
	size_t height = pDepthStencil->getHeight();
	if (customViewport.has_value()) {
		graphicsCtx.setViewport(*customViewport);
	} else {
		graphicsCtx.setViewport(D3D12_VIEWPORT {
			.TopLeftX = 0.f,
			.TopLeftY = 0.f,
			.Width = static_cast<FLOAT>(width),
			.Height = static_cast<FLOAT>(height),
			.MinDepth = 0.f,
			.MaxDepth = 1.f,
		});
	}

	if (customScissorRect.has_value()) {
		graphicsCtx.setScissorRect(*customScissorRect);
	} else {
		graphicsCtx.setScissorRect(D3D12_RECT{
			.left = 0,
			.top = 0,
			.right = static_cast<LONG>(width),
			.bottom = static_cast<LONG>(height)
		});
	}

	if (pRenderTarget != nullptr) {
		graphicsCtx.setRenderTarget(
			pRenderTarget->getRTV(renderTargetMipmap),
			pDepthStencil->getDSV()
		);
	} else {
		graphicsCtx.setRenderTarget(pDepthStencil->getDSV());
	}
}

DXGI_FORMAT ExecutablePass::getRtFormat(size_t slot) const {
	return pRenderTarget->getFormat();
}

DXGI_FORMAT ExecutablePass::getDsFormat() const {
	return pDepthStencil->getFormat();
}

void ExecutablePass::reset() {
	Pass::reset();
	for (auto *pPassResource : _passResources) {
		if (pPassResource->isActivated()) {
			pPassResource->setFinished(false);
			pPassResource->reset();
		}
	}
}

void ExecutablePass::addPassResource(PassResourceBase *pResource) {
	_passResources.push_back(pResource);
}

const std::vector<PassResourceBase *> & ExecutablePass::getPassResources() const {
	return _passResources;
}

}
