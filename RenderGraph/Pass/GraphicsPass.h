#pragma once
#include <RenderGraph/Pass/ExecutablePass.h>

namespace rgph {

class GraphicsPass : public ExecutablePass {
public:
	GraphicsPass(const std::string &passName, bool rtActive = true, bool dsActive = true);
	void preExecute(dx12lib::DirectContextProxy pDirectCtx) override;
	void execute(dx12lib::DirectContextProxy pDirectCtx) override;
	void postExecute(dx12lib::DirectContextProxy pDirectCtx) override;
	PassType getPassType() const override;
	void bindRenderTarget(dx12lib::IGraphicsContext &graphicsCtx);
	DXGI_FORMAT getRtFormat() const;
	DXGI_FORMAT getDsFormat() const;
public:
	size_t renderTargetMipmap = 0;
	std::optional<D3D12_RECT> customScissorRect;
	std::optional<D3D12_VIEWPORT> customViewport;
	PassResourcePtr<dx12lib::RenderTarget2D> pRenderTarget;
	PassResourcePtr<dx12lib::DepthStencil2D> pDepthStencil;
};

}
