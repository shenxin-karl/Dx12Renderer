#pragma once
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/Buffer/FRConstantBuffer.hpp>

namespace rgph {

struct TransformStore {
	float4x4 matWorld;
	float4x4 matNormal;
};

class TransformCBufferPtr {
public:
	const float4x4 &getMatWorld() const;
	const float4x4 &getMatNormal() const;
	const TransformStore &getTransformStore() const;
	void setTransformStore(const TransformStore &store);
	void setTransformCBuffer(FRConstantBufferPtr<TransformStore> pTransformCBuf);
	void bind(dx12lib::IGraphicsContext &graphicsCtx, const dx12lib::ShaderRegister &shaderRegister) const;
protected:
	FRConstantBufferPtr<TransformStore> _pTransformCBuf;
};

}
