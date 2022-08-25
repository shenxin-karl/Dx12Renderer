#include "Job.h"
#include "Geometry.h"
#include "TransformCBuffer.h"
#include "RenderGraph/Bindable/GraphicsPSOBindable.h"
#include "RenderGraph/Drawable/Drawable.h"
#include "RenderGraph/Technique/Step.h"

namespace rgph {

Job::Job(const Step *pStep, const Geometry *pGeometry, const TransformCBuffer *pTransformCBuffer)
:  pStep(pStep), pGeometry(pGeometry), pTransformCBuffer(pTransformCBuffer)
{
	assert(pStep != nullptr);
	assert(pGeometry != nullptr);
	assert(pTransformCBuffer != nullptr);
}

void Job::execute(dx12lib::IGraphicsContext &graphicsCtx, 
	const VertexInputSlot &vertexInputSlot,
	const dx12lib::ShaderRegister &transformCBufferReg) const
{
	assert(pStep != nullptr);
	assert(pGeometry != nullptr);
	assert(pTransformCBuffer != nullptr);

	pGeometry->bind(graphicsCtx, vertexInputSlot);
	pStep->bind(graphicsCtx);
	pTransformCBuffer->bind(graphicsCtx, transformCBufferReg);
	pGeometry->draw(graphicsCtx);
}
}
