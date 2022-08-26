#pragma once
#include <RenderGraph/Pass/BindingPass.h>
#include <RenderGraph/Job/Job.h>
#include <Dx12lib/Pipeline/ShaderRegister.hpp>

namespace rgph {

class SubPass;
class RenderQueuePass : public BindingPass {
public:
	using BindingPass::BindingPass;
	std::shared_ptr<SubPass> getSubPassByName(const std::string &subPassName) const;
	void addSubPass(std::shared_ptr<SubPass> pSubPass);
	void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const override;
	void reset() override;
	void setPassCBuffer(std::shared_ptr<dx12lib::IConstantBuffer> pCBuffer);
private:
	std::vector<std::shared_ptr<SubPass>> _subPasses;
	std::shared_ptr<dx12lib::IConstantBuffer> _pPassCBuffer;
};

}
