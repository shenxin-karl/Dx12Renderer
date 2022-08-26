#include "RenderQueuePass.h"
#include "RenderGraph/Bindable/Bindable.hpp"
#include "RenderGraph/Bindable/GraphicsPSOBindable.h"
#include "RenderGraph/Pass/SubPass.h"

namespace rgph {

std::shared_ptr<SubPass> RenderQueuePass::getSubPassByName(const std::string &subPassName) const {
	for (auto &pSubPass : _subPasses) {
		if (pSubPass->getSubPassName() == subPassName)
			return pSubPass;
	}
	return nullptr;
}

void RenderQueuePass::addSubPass(std::shared_ptr<SubPass> pSubPass) {
	assert(pSubPass == nullptr);
	const std::string &subPassName = pSubPass->getSubPassName();
	for (auto &subPassPtr : _subPasses) {
		if (subPassPtr->getSubPassName() == subPassName) {
			assert(false);
			return;
		}
	}
	_subPasses.push_back(std::move(pSubPass));
}


void RenderQueuePass::execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const {
	bool hasJob = true;
	for (auto &pSubPass : _subPasses)
		hasJob = hasJob && (pSubPass->getJobCount() > 0);

	if (!hasJob)
		return;

	bindAll(*pGraphicsCtx);
	bindRenderTarget(*pGraphicsCtx);
	for (auto &pSubPass : _subPasses) {
		auto passCBufferShaderRegister = pSubPass->getPassCBufferShaderRegister();
		if (_pPassCBuffer == nullptr && passCBufferShaderRegister.slot && !passCBufferShaderRegister.slot.isSampler())
			pGraphicsCtx->setConstantBuffer(passCBufferShaderRegister, _pPassCBuffer);
		pSubPass->execute(*pGraphicsCtx);
	}
}

void RenderQueuePass::reset() {
	BindingPass::reset();
	for (auto &pSubPass : _subPasses)
		pSubPass->reset();
}

void RenderQueuePass::setPassCBuffer(std::shared_ptr<dx12lib::IConstantBuffer> pCBuffer) {
	_pPassCBuffer = std::move(pCBuffer);
}

}
