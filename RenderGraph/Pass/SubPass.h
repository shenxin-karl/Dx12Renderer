#pragma once
#include <vector>
#include <Dx12lib/Pipeline/ShaderRegister.hpp>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <RenderGraph/Job/Geometry.h>
#include <RenderGraph/Job/Job.h>

namespace rgph {

class SubPass {
public:
	SubPass(std::shared_ptr<GraphicsPSOBindable> pGraphicsBindable);
	const std::string &getSubPassName() const;
	void addBindable(std::shared_ptr<Bindable> pBindable);
	std::shared_ptr<Bindable> getBindableByType(BindableType bindableType) const;
	virtual void accept(const Job &job);
	virtual void execute(dx12lib::IGraphicsContext &graphicsCtx) const;
	virtual void reset();
	virtual ~SubPass();
	size_t getJobCount() const;
	void setVertexDataInputSlots(const VertexInputSlot &inputSlot);
	void setTransformCBufferShaderRegister(const dx12lib::ShaderRegister &transShaderRegister);
	void setPassCBufferShaderRegister(const dx12lib::ShaderRegister &passShaderRegister);
	const VertexInputSlot &getVertexDataInputSlots() const;
	const dx12lib::ShaderRegister &getTransformCBufferShaderRegister() const;
	const dx12lib::ShaderRegister &getPassCBufferShaderRegister() const;
protected:
	bool _finalize = false;
	std::vector<Job> _jobs;
	VertexInputSlot _vertexDataSlots;
	dx12lib::ShaderRegister _transformCBufferShaderRegister;
	dx12lib::ShaderRegister _passCBufferShaderRegister;
	std::shared_ptr<GraphicsPSOBindable> _pGraphicsPsoBindable;
	std::vector<std::shared_ptr<Bindable>> _bindables;
};

}
