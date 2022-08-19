#pragma once
#include <RenderGraph/Pass/BindingPass.h>
#include <Dx12lib/Texture/RenderTargetTexture.h>

#include "Dx12lib/Texture/DepthStencilTexture.h"

namespace rg {

class ClearRtPass : public BindingPass {
public:
	ClearRtPass(const std::string &passName) : BindingPass(passName) {
		pRenderTarget.preExecuteState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}

	void link(dx12lib::ICommonContext &commonCtx) const override {
		assert(pRenderTarget != nullptr);
		pRenderTarget.link(commonCtx);
	}

	void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const override {
		assert(pDepthStencil == nullptr);
		link(*pGraphicsCtx);
		pGraphicsCtx->clearColor(pRenderTarget, pRenderTarget->getClearValue().Color);
	}
};

class CLearDsPass : public BindingPass {
public:
	CLearDsPass(const std::string &passName) : BindingPass(passName) {
		pDepthStencil.preExecuteState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}

	void link(dx12lib::ICommonContext &commonCtx) const override {
		assert(pDepthStencil != nullptr);
		pDepthStencil.link(commonCtx);
	}

	void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const override {
		assert(pRenderTarget == nullptr);
		link(*pGraphicsCtx);
		auto clearValue = pDepthStencil->getClearValue();
		pGraphicsCtx->clearDepthStencil(
			pDepthStencil,
			clearValue.DepthStencil.Depth,
			clearValue.DepthStencil.Stencil
		);
	}
};

}
