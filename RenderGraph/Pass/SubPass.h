#pragma once
#include <vector>
#include <Dx12lib/Pipeline/ShaderRegister.hpp>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <RenderGraph/Job/Geometry.h>
#include <RenderGraph/Job/Job.h>

namespace rgph {

class SubPass {
public:
	SubPass(std::weak_ptr<dx12lib::GraphicsPSO> pGraphicsPso);
	const std::string &getSubPassName() const;
	void addBindable(std::shared_ptr<Bindable> pBindable);
	std::shared_ptr<Bindable> getBindableByType(BindableType bindableType) const;
	virtual void bind(dx12lib::IGraphicsContext &graphicsCtx) const;
	virtual void accept(const Job &job);
	virtual void execute(dx12lib::IGraphicsContext &graphicsCtx) const;
	virtual void reset();
	virtual ~SubPass();
	size_t getJobCount() const;
	void setVertexDataInputSlots(const VertexInputSlots &inputSlot);
	void setTransformCBufferShaderRegister(const dx12lib::ShaderRegister &transShaderRegister);
	void setPassCBufferShaderRegister(const dx12lib::ShaderRegister &passShaderRegister);
	const VertexInputSlots &getVertexDataInputSlots() const;
	const dx12lib::ShaderRegister &getTransformCBufferShaderRegister() const;
	const dx12lib::ShaderRegister &getPassCBufferShaderRegister() const;
	bool valid() const;
protected:
	bool _finalize = false;
	std::vector<Job> _jobs;
	std::string _subPassName;
	VertexInputSlots _vertexDataSlots;
	dx12lib::ShaderRegister _transformCBufferShaderRegister;
	dx12lib::ShaderRegister _passCBufferShaderRegister;
	std::weak_ptr<dx12lib::GraphicsPSO> _pGraphicsPso;
	std::vector<std::shared_ptr<Bindable>> _bindables;
};

}
