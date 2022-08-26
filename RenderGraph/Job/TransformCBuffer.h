#pragma once
#include <RenderGraph/RenderGraphStd.h>

namespace rgph {

struct TransformStore {
	float4x4 matWorld;
	float4x4 matNormal;
	float4x4 matWVP;
};

class TransformCBuffer {
public:
	const float4x4 &getMatWorld() const;
	const float4x4 &getMatNormal() const;
	const float4x4 &getMatWVP() const;
	const AxisAlignedBox &getWorldAABB() const;
	const TransformStore &getTransformStore() const;
	void setTransformStore(const TransformStore &store);
	void setWorldAABB(const AxisAlignedBox &box);
	void setTransformCBuf(FRConstantBufferPtr<TransformStore> pTransformCBuf);
	void bind(dx12lib::IGraphicsContext &graphicsCtx, const dx12lib::ShaderRegister &shaderRegister) const;
protected:
	AxisAlignedBox _worldAABB;
	FRConstantBufferPtr<TransformStore> _pTransformCBuf;
};

}
