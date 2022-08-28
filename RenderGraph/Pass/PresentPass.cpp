#pragma once
#include <RenderGraph/Pass/ExecutablePass.h>

namespace rgph {

class PresentPass : public ExecutablePass {
public:
	PresentPass(const std::string &passName) : ExecutablePass(passName) {
		pRenderTarget.preExecuteState = D3D12_RESOURCE_STATE_PRESENT;
	}

	void preExecute(dx12lib::DirectContextProxy pDirectCtx) override {
		assert(pRenderTarget != nullptr);
		pRenderTarget.link(*pDirectCtx);
	}

};

}
