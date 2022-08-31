#pragma once
#include <RenderGraph/Pass/GraphicsPass.h>

namespace rgph {

class PresentPass : public GraphicsPass {
public:
	PresentPass(const std::string &passName) : GraphicsPass(passName, true, false) {
		pRenderTarget.preExecuteState = D3D12_RESOURCE_STATE_PRESENT;
	}

	void preExecute(dx12lib::DirectContextProxy pDirectCtx) override {
		assert(pRenderTarget != nullptr);
		pRenderTarget.link(*pDirectCtx);
	}

};

}
