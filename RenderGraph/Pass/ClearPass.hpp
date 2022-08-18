#pragma once
#include <RenderGraph/Pass/BindingPass.h>
#include <Dx12lib/Texture/RenderTargetTexture.h>

namespace rg {

class ClearRtPass : public BindingPass {
public:
	using BindingPass::BindingPass;

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
	using BindingPass::BindingPass;

	void link(dx12lib::ICommonContext &commonCtx) const override {
		assert(pDepthStencil != nullptr);
		pDepthStencil.link(commonCtx);
	}

	void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const override {
		assert(pRenderTarget == nullptr);
		link(*pGraphicsCtx);
		pGraphicsCtx->clearColor(pRenderTarget, pRenderTarget->getClearValue().Color);
	}
};

}
