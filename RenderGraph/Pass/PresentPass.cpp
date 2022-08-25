#pragma once
#include <RenderGraph/Pass/BindingPass.h>

namespace rgph {

class PresentPass : public BindingPass {
public:
	PresentPass(const std::string &passName) : BindingPass(passName) {
		pRenderTarget.preExecuteState = D3D12_RESOURCE_STATE_PRESENT;
	}

	void link(dx12lib::ICommonContext &commonCtx) const override {
		assert(pRenderTarget != nullptr);
		pRenderTarget.link(commonCtx);
	}

	void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const override {
		link(*pGraphicsCtx);
	}
};

}
