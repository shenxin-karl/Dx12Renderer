#pragma once
#include <RenderGraph/RenderGraphStd.h>

namespace rgph {

interface IMesh : public NonCopyable {
	using BoneIndex = std::array<uint8_t, 4>;
	virtual const std::vector<float4> &getPositions() const { static std::vector<float4> vec; return vec; }
	virtual const std::vector<float3> &getNormals() const { static std::vector<float3> vec; return vec; }
	virtual const std::vector<float3> &getTangents() const { static std::vector<float3> vec; return vec; }
	virtual const std::vector<float2> &getTexcoord0() const { static std::vector<float2> vec; return vec; }
	virtual const std::vector<float2> &getTexcoord1() const { static std::vector<float2> vec; return vec; }
	virtual const std::vector<BoneIndex> &getBoneIndices() const { static std::vector<BoneIndex> vec; return vec; }
	virtual const std::vector<float3> &getBoneWeight() const { static std::vector<float3> vec; return vec; }
	virtual const std::vector<uint32_t> &getIndices() const { static std::vector<uint32_t> vec; return vec; }
	virtual const AxisAlignedBox &getBoundingBox() const { static AxisAlignedBox box; return box; }
};

using VertexInputSlot = std::bitset<dx12lib::kVertexBufferSlotCount>;

struct DrawArgs {
	size_t vertexCount = 0;
	size_t baseVertexLocation = 0;
	size_t indexCount = 0;
	size_t startIndexLocation = 0;
	size_t instanceCount = 1;
	size_t startInstanceLocation = 0;
};

class Geometry {
public:
	void bind(dx12lib::IGraphicsContext &graphicsCtx, const VertexInputSlot &vertexInputSlot) const;
	void setVertexBuffer(size_t idx, std::shared_ptr<dx12lib::VertexBuffer> pVertexBuffer);
	void setIndexBuffer(std::shared_ptr<dx12lib::IndexBuffer> pIndexBuffer);
	void setMesh(std::shared_ptr<IMesh> pMesh);
	std::shared_ptr<dx12lib::VertexBuffer> getVertexBuffer(size_t idx) const;
	std::shared_ptr<dx12lib::IndexBuffer> getIndexBuffer() const;
	std::shared_ptr<IMesh> getMesh() const;
	void setDrawArgs(const DrawArgs &drawArgs);
	void setTopology(D3D_PRIMITIVE_TOPOLOGY topology);
	const DrawArgs &getDrawArgs() const;
	D3D_PRIMITIVE_TOPOLOGY getTopology() const;
	void genDrawArgs();
	void draw(dx12lib::IGraphicsContext &graphicsCtx) const;
protected:
	DrawArgs _drawArgs;
	std::shared_ptr<IMesh> _pMesh;
	std::shared_ptr<dx12lib::IndexBuffer> _pIndexBuffer;
	std::shared_ptr<dx12lib::VertexBuffer> _pVertexBufferList[dx12lib::kVertexBufferSlotCount];
	D3D_PRIMITIVE_TOPOLOGY _topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
};

}
